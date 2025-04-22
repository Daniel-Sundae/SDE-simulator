#include "PathEngine.hpp"
#include "PathQuery.hpp"
#include "PDFQuery.hpp"
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

auto PathEngine::SamplePathFunctor(const PathQuery& pathQuery, const std::size_t slot, const std::uint32_t seed) -> std::function<void()>
{
    return [this, pathQuery, slot, seed]() -> void {
        std::mt19937 generator(seed);
        Path path = SampleOnePathImpl(pathQuery, generator);

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

auto PathEngine::SampleOnePath(const PathQuery& pathQuery) const -> Path
{
    const std::uint32_t seed = pathQuery.settingsParameters.useSeed.first ? pathQuery.settingsParameters.useSeed.second : std::random_device{}();
    std::mt19937 generator(seed);
    return SampleOnePathImpl(pathQuery, generator);
}

auto PathEngine::SampleOnePathImpl(const PathQuery& pathQuery, std::mt19937& generator) const -> Path
{
    const std::size_t points = pathQuery.simulationParameters.Points();
    const auto& drift = pathQuery.processDefinition.drift;
    const auto& diffusion = pathQuery.processDefinition.diffusion;
    const Time dt = pathQuery.simulationParameters.dt;
    const State startValueData = pathQuery.processDefinition.startValueData;
    Path path = {};
    assert(points != 0);
    path.reserve(points);
    path.push_back(startValueData);
    for (std::size_t i = 1; i < points; ++i) {
        if (i % 100 == 0 && m_cancelRequested.load()) {
            return {};
        }
        path.push_back(path.back() + Increment(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt, generator));
    }
    return path;
}

auto PathEngine::SamplePathsAsync(const PathQuery& pathQuery, std::function<void(Paths)> onCompletionCb) -> void
{
    assert(!IsBusy() && "Cannot query busy engine");
    m_engineSettings = pathQuery.settingsParameters;
    m_cancelRequested = false;
    m_completedTasks = 0;
    
    auto mainTask = [this, pathQuery, onCompletionCb]() -> void{

        // Prepare results vector
        const std::size_t nrSamples = pathQuery.simulationParameters.samples;
        assert(m_tp->NrBusyThreads() == 1 && "Only mainTask should be running");
        m_pathResults.clear();
        m_pathResults.resize(nrSamples);
        for (std::size_t i = 0; i < nrSamples; ++i) {
            m_pathResults[i].resize(pathQuery.simulationParameters.Points());
            // Provide unique generator for each task
            const std::uint32_t seed = pathQuery.settingsParameters.useSeed.first ? pathQuery.settingsParameters.useSeed.second + i :
                static_cast<std::uint32_t>(std::random_device{}());
            m_engineSettings.useThreading ? m_tp->Enqueue(SamplePathFunctor(pathQuery, i, seed)) : SamplePathFunctor(pathQuery, i, seed)();
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
        m_tp->ClearTasks();
        m_pathResults.clear();
    };
    m_tp->Enqueue(mainTask);
}

auto PathEngine::RequestCancel() -> void
{
    {
        std::scoped_lock lock(m_completionMtx); // Ensures mainTask reads updated atomic bool
        m_cancelRequested = true;
    }
    m_completionCv.notify_one();
}

auto PathEngine::IsBusy() -> bool
{
    return m_tp->NrBusyThreads() ? true : false;
}

auto PathEngine::Increment(
    const Drift& drift,
    const Diffusion& diffusion,
    const Time t,
    const State Xt,
    const Time dt,
    std::mt19937& generator) const -> State
{
    const double dB = [&generator, dt](){
        double stdev = std::sqrt(dt);
        std::normal_distribution<> d(0.0, stdev);
        return d(generator);
    }();
    return drift(t, Xt) * dt + diffusion(t, Xt) * dB;
}