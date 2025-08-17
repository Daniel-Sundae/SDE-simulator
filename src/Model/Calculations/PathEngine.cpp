#include "PathEngine.hpp"
#include "PathQuery.hpp"
#include "SolverData.hpp"
#include "Transaction.hpp"
#include <thread>
#include <cassert>

template <typename F>
static std::optional<Path> sampleOnePathImpl(const PathQuery& pathQuery, std::mt19937& generator, F dXt, std::stop_token stopToken) {
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
        if (i % 10 == 0 && stopToken.stop_requested()){
            return std::nullopt;
        }
        path.push_back(path.back() + dXt(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt, generator));
    }
    return path;
}

static auto createPathSamplingFunction(const PathQuery& query, uint32_t seedId, std::stop_token token, std::shared_ptr<std::atomic<size_t>> pathsCompleted) {
    uint32_t seed = query.settingsParameters.useSeed ? uint32_t(query.settingsParameters.useSeed.value()) : std::random_device{}();
    seed += seedId; // Ensure unique seed for each task
    return [generator = std::mt19937(seed),
        query,
        token,
        pathsCompleted]
        () mutable -> Path {
            auto path = sampleOnePathImpl(query, generator, dXtFunctor(query.simulationParameters.solver), token);
            if (path) pathsCompleted->fetch_add(1, std::memory_order_relaxed);
            return path.value_or(Path{});
    };
}

static std::future<Paths> launchPathSampling(const Job& job, const PathQuery& query, EngineThreadPool* threadpool){
    return std::async(std::launch::async,
        [jobStatus = job.status, stopToken = job.stop.get_token(), pathsCompleted = job.pathsCompleted, query, threadpool]() mutable -> Paths {
            *jobStatus = Job::Status::Running;
            const size_t nrPaths = query.simulationParameters.samples;
            std::vector<std::future<Path>> pathFutures;
            pathFutures.reserve(nrPaths);
            Paths paths{};
            paths.reserve(nrPaths);
            for (uint32_t i = 0; i < nrPaths; ++i) {
                pathFutures.push_back(threadpool->enqueue(createPathSamplingFunction(query, i, stopToken, pathsCompleted)));
            }
            for (auto& future : pathFutures) {
                paths.push_back(future.get());
            }
            *jobStatus = stopToken.stop_requested() ? Job::Status::Cancelled : Job::Status::Completed;
            return paths;
    });
}

[[nodiscard]] Job PathEngine::processPathQuery(const PathQuery& pathQuery) {
    Job job{
        pathQuery.simulationParameters.samples,
        pathQuery.processDefinition.diffusion.isZero() ? Job::Type::Deterministic : Job::Type::Stochastic};
    job.setResult(launchPathSampling(job, pathQuery, m_tp.get()));
    return job;
}

bool PathEngine::isBusy(){
    return m_tp->nrBusyThreads() != 0;
}