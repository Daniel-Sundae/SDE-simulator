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

std::function<Path()> PathEngine::createPathSamplingFunction(const PathQuery& pQuery, uint32_t seedId) const{
    const auto seedOpt = pQuery.settingsParameters.useSeed;
    const auto solver = pQuery.simulationParameters.solver;
    uint32_t seed = seedOpt ? uint32_t(seedOpt.value()) : std::random_device{}();
    seed += seedId; // Ensure unique seed for each task
    return [=]() -> Path {
        std::mt19937 generator(seed);
        return sampleOnePathImpl(pQuery, generator, dXtFunctor(solver));
    };
}

template <typename F>
Path PathEngine::sampleOnePathImpl(const PathQuery& pathQuery, std::mt19937& generator, F dXt) const{
    const auto points = pathQuery.simulationParameters.points();
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

[[nodiscard]] std::future<Paths> PathEngine::samplePathsAsync(const PathQuery& pQuery){
    assert(!isBusy() && "Cannot query busy engine");
    m_engineSettings = pQuery.settingsParameters;
    m_cancelRequested = false;
    m_completedTasks = 0;
    
    auto taskCoordinator = [this, pQuery]() -> Paths{
        if(size_t threads = m_tp->nrBusyThreads(); threads) {
            Utils::fatalError("EngineThreadPool has {} busy threads before sampling.", threads);
        }
        const size_t nrPaths = pQuery.simulationParameters.samples;
        std::vector<std::future<Path>> pathFutures;
        pathFutures.reserve(nrPaths);
        for (uint32_t i = 0; i < nrPaths; ++i) {
            pathFutures.push_back(m_tp->enqueue(createPathSamplingFunction(pQuery, i)));
        }

        Paths paths;
        paths.reserve(nrPaths);
        for (auto& future : pathFutures) {
            paths.push_back(future.get());
        }
        if(size_t threads = m_tp->nrBusyThreads(); threads) {
            Utils::fatalError("EngineThreadPool has {} busy threads after sampling.", threads);
        }
        return paths;
    };
    return std::async(std::launch::async, taskCoordinator);
}

void PathEngine::requestCancel(){
    {
        std::scoped_lock lock(m_completionMtx); // Ensures mainTask reads updated atomic bool
        m_cancelRequested = true;
    }
    m_completionCv.notify_one();
}

bool PathEngine::isBusy(){
    return m_tp->nrBusyThreads() ? true : false;
}