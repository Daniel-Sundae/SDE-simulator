#include "PathEngine.hpp"
#include "PathQuery.hpp"
#include "SolverData.hpp"
#include <thread>
#include <cassert>

static std::atomic<bool> s_cancelRequested{false};

template <typename F>
static Path sampleOnePathImpl(const PathQuery& pathQuery, std::mt19937& generator, F dXt, std::stop_token stopToken) {
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
        if (i % 10 == 0 && stopToken.stop_requested()) {
            return {};
        }
        path.push_back(path.back() + dXt(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt, generator));
    }
    return path;
}

static auto createPathSamplingFunction(const PathQuery& pQuery, uint32_t seedId, std::stop_token stopToken){
    const auto seedOpt = pQuery.settingsParameters.useSeed;
    const auto solver = pQuery.simulationParameters.solver;
    uint32_t seed = seedOpt ? uint32_t(seedOpt.value()) : std::random_device{}();
    seed += seedId; // Ensure unique seed for each task
    std::mt19937 generator(seed);
    return [generator, pQuery, solver, stopToken]() mutable -> Path {
        return sampleOnePathImpl(pQuery, generator, dXtFunctor(solver), stopToken);
    };
}

[[nodiscard]] Job PathEngine::samplePathsAsync(const PathQuery& pQuery){
    Job job{};
    job.progress.totalTasks = pQuery.simulationParameters.samples;
    auto stopToken = job.stop.get_token();
    job.result = std::async(std::launch::async, [pQuery, stopToken]() mutable -> Paths {
        const size_t nrPaths = pQuery.simulationParameters.samples;
        std::vector<std::future<Path>> pathFutures;
        pathFutures.reserve(nrPaths);
        Paths paths{};
        paths.reserve(nrPaths);
        for (size_t i = 0; i < nrPaths; ++i) {
            pathFutures.push_back(m_tp->enqueue(createPathSamplingFunction(pQuery, i, stopToken)));
        }
        for (auto& future : pathFutures) {
            paths.push_back(future.get());
        }
        return paths;
    });
    return job;
}

void PathEngine::requestCancel(){
    s_cancelRequested = true;
}

bool PathEngine::isBusy(){
    return m_tp->nrBusyThreads() != 0;
}