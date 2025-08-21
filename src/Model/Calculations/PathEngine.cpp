#include "PathEngine.hpp"
#include "PathQuery.hpp"
#include "SolverData.hpp"
#include "Transaction.hpp"
#include <thread>
#include <cassert>

template <typename F>
static Path samplePath(const PathQuery& pathQuery, std::mt19937& generator, F dXt, std::stop_token stopToken) {
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
        if (i % 10 == 0 && stopToken.stop_requested()) return {};
        path.push_back(path.back() + dXt(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt, generator));
    }
    return path;
}

static Paths samplePaths(
        const PathQuery& query,
        EngineThreadPool* threadpool,
        std::shared_ptr<std::atomic<Job::Status>> jobStatus,
        std::shared_ptr<std::atomic<size_t>> pathsCompleted,
        std::stop_token stopToken){
    *jobStatus = Job::Status::Running;
    const size_t nrPaths = query.simulationParameters.samples;
    std::vector<std::future<Path>> pathFutures;
    pathFutures.reserve(nrPaths);
    Paths paths{};
    paths.reserve(nrPaths);
    uint32_t seed = query.settingsParameters.useSeed ? uint32_t(query.settingsParameters.useSeed.value()) : std::random_device{}();
    auto dXt = dXtFunctor(query.simulationParameters.solver);
    for (uint32_t _ = 0; _ < nrPaths; ++_) {
        seed += 1; // Ensure unique seed for each task
        pathFutures.push_back(threadpool->enqueue(
            [query, generator = std::mt19937(seed), dXt, stopToken, pathsCompleted]() mutable -> Path {
                Path path = stopToken.stop_requested() ? Path{} : samplePath(query, generator, dXt, stopToken);
                pathsCompleted->fetch_add(1, std::memory_order_relaxed);
                return path;
            }
        ));
    }
    for (auto& future : pathFutures) {
        paths.push_back(future.get());
    }
    *jobStatus = stopToken.stop_requested() ? Job::Status::Cancelled : Job::Status::Completed;
    return paths;
}

[[nodiscard]] Job PathEngine::processPathQuery(const PathQuery& pathQuery) {
    Job job{
        pathQuery.simulationParameters.samples,
        pathQuery.processDefinition.diffusion.isZero() ? Job::Type::Deterministic : Job::Type::Stochastic
    };
    job.result = std::move(std::async(std::launch::async, samplePaths,
        pathQuery, m_tp.get(), job.status, job.pathsCompleted, job.stop.get_token()));
    return job;
}

bool PathEngine::isBusy(){
    return m_tp->nrBusyThreads() != 0;
}