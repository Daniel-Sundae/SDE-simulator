#include "PathEngine.hpp"
#include "PathQuery.hpp"
#include "SolverData.hpp"
#include <thread>
#include <cassert>

PathEngine::PathEngine()
    : m_tp(std::make_unique<EngineThreadPool>())
    , m_pathResults()
    , m_pathsMtx()
    , m_cancelRequested(false)
    , m_completedTasks(0)
    , m_completionCv()
    , m_completionMtx()
    , m_engineSettings()
{ }

std::function<void()> PathEngine::samplePathFunctor(const PathQuery& pathQuery, const size_t slot, const size_t seed){
    return [this, pathQuery, slot, seed]() -> void {
        std::mt19937 generator(static_cast<uint32_t>(seed));
        Path path = sampleOnePathImpl(pathQuery, generator, dXtFunctor(pathQuery.simulationParameters.solver));

        // Check before writing
        if (m_cancelRequested) return;
        
        // Don't need to protect vector since each thread 
        // is given unique slot with preallocated space
        m_pathResults[slot] = std::move(path);

        // Important to lock atomic for two reasons:
        // 1: Unlocking guarantees increment is visible for mainTask thread before checking predicate. Prevents false re-sleep.
        // 2: Holding mtx guarantees mainTask is listening for wakeup. Prevent lost wake-up
        {
            std::scoped_lock lock(m_completionMtx);
            m_completedTasks.fetch_add(1);
        }
        m_completionCv.notify_one();
    };
}

Path PathEngine::sampleOnePath(const PathQuery& pathQuery) const{
    auto& hasSeed = pathQuery.settingsParameters.useSeed;
    const size_t seed = hasSeed ? hasSeed.value() : std::random_device{}();
    std::mt19937 generator(static_cast<uint32_t>(seed));
    return sampleOnePathImpl(pathQuery, generator, dXtFunctor(pathQuery.simulationParameters.solver));
}

template <typename F>
Path PathEngine::sampleOnePathImpl(const PathQuery& pathQuery, std::mt19937& generator, F dXt) const{
    const size_t points = pathQuery.simulationParameters.points();
    const auto& drift = pathQuery.processDefinition.drift;
    const auto& diffusion = pathQuery.processDefinition.diffusion;
    const Time dt = pathQuery.simulationParameters.dt;
    const State startValueData = pathQuery.processDefinition.startValueData;
    Path path = {};
    assert(points != 0);
    path.reserve(points);
    path.push_back(startValueData);
    for (size_t i = 1; i < points; ++i) {
        if (i % 100 == 0 && m_cancelRequested.load()) {
            return {};
        }
        path.push_back(path.back() + dXt(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt, generator));
    }
    return path;
}

void PathEngine::samplePathsAsync(const PathQuery& pathQuery, std::function<void(Paths)> onCompletionCb){
    assert(!isBusy() && "Cannot query busy engine");
    m_engineSettings = pathQuery.settingsParameters;
    m_cancelRequested = false;
    m_completedTasks = 0;
    
    auto mainTask = [this, pathQuery, onCompletionCb]() -> void{

        // Prepare results vector
        const size_t nrSamples = pathQuery.simulationParameters.samples;
        assert(m_tp->nrBusyThreads() == 1 && "Only mainTask should be running");
        m_pathResults.clear();
        m_pathResults.resize(nrSamples);
        for (size_t i = 0; i < nrSamples; ++i) {
            m_pathResults[i].resize(pathQuery.simulationParameters.points());
            // Provide unique generator for each task
            auto& seedOpt = pathQuery.settingsParameters.useSeed;
            const size_t seed = seedOpt ? seedOpt.value() + i
                : static_cast<size_t>(std::random_device{}());
            m_engineSettings.useThreading ? m_tp->enqueue(samplePathFunctor(pathQuery, i, seed)) : samplePathFunctor(pathQuery, i, seed)();
        }

        // Wait until tasks done or GUI thread cancels
        std::unique_lock<std::mutex> completionLock(m_completionMtx);
        m_completionCv.wait(completionLock, [this, nrSamples]() {return m_completedTasks == nrSamples || m_cancelRequested;});

        // Extract result (if any)
        Paths returnVal;
        {
            std::scoped_lock sl(m_pathsMtx);
            returnVal = m_cancelRequested.load() ? Paths{} : std::move(m_pathResults);
        }
        onCompletionCb(std::move(returnVal));

        // Clearing is relevant if sampling was cancelled
        m_tp->clearTasks();
        m_pathResults.clear();
    };
    m_tp->enqueue(mainTask);
}

void PathEngine::requestcancel(){
    {
        std::scoped_lock lock(m_completionMtx); // Ensures mainTask reads updated atomic bool
        m_cancelRequested = true;
    }
    m_completionCv.notify_one();
}

bool PathEngine::isBusy(){
    return m_tp->nrBusyThreads() ? true : false;
}