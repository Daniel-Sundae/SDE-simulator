#include "PathEngine.hpp"
#include "PathQuery.hpp"
#include "SolverData.hpp"
#include "Transaction.hpp"
#include <thread>
#include <cassert>

static Path samplePath(const PathQuery& query, std::mt19937& generator, std::stop_token stopToken) {
    const auto points = query.simulationParameters.points();
    const auto& drift = query.processDefinition.drift;
    const auto& diffusion = query.processDefinition.diffusion;
    const Time dt = query.simulationParameters.dt;
    const State startValueData = query.processDefinition.startValueData;
    auto dXt = dXtFunctor(query.simulationParameters.solver);
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
        EngineThreadPool& threadpool,
        std::shared_ptr<std::atomic<size_t>> pathsCompleted,
        std::stop_token stopToken){
    const size_t samples = query.simulationParameters.samples;
    Paths paths{};
    paths.reserve(samples);
    const uint32_t baseSeed = query.settingsParameters.useSeed
        ? uint32_t(query.settingsParameters.useSeed.value())
        : std::random_device{}();
    auto _samplePath = [query, baseSeed, stopToken, pathsCompleted](uint32_t seedOffset) mutable -> Path {
        std::seed_seq seq{ baseSeed, seedOffset };
        std::mt19937 generator(seq);
        Path path = stopToken.stop_requested() ? Path{} : samplePath(query, generator, stopToken);
        pathsCompleted->fetch_add(1, std::memory_order_relaxed);
        return path;
    };
    if(query.settingsParameters.useThreading){
        std::vector<std::future<Path>> pathFutures;
        pathFutures.reserve(samples);
        for (uint32_t i = 0; i < samples; ++i) {
            pathFutures.push_back(threadpool.enqueue([_samplePath, i]() mutable {return _samplePath(i);}));
        }
        for (auto& future : pathFutures) {
            paths.push_back(future.get());
        }
    } else {
        for (uint32_t i = 0; i < samples; ++i) {
            paths.push_back(_samplePath(i));
        }
    }

    return paths;
}

[[nodiscard]] Job PathEngine::processPathQuery(const PathQuery& pathQuery) {
    Job job{
        pathQuery.simulationParameters.samples,
        pathQuery.processDefinition.diffusion.isZero() ? Job::Type::Deterministic : Job::Type::Stochastic
    };
    job.result = std::move(std::async(std::launch::async, samplePaths,
        pathQuery, std::ref(*m_tp), job.pathsCompleted, job.stop.get_token()));
    return job;
}

bool PathEngine::isBusy(){
    return m_tp->nrBusyThreads() != 0;
}