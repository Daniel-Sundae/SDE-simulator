#include "PathEngine.hpp"
#include "PathQuery.hpp"
#include "SolverData.hpp"
#include "Transaction.hpp"
#include <thread>

static Path samplePath(
        const PathQuery& query,
        std::seed_seq seq,
        std::stop_token stopToken) {
    if (stopToken.stop_requested()) return {};
    const auto points = query.simulationParameters.points();
    const auto& drift = query.processDefinition.drift;
    const auto& diffusion = query.processDefinition.diffusion;
    const Time dt = query.simulationParameters.dt;
    const State startValue = query.processDefinition.startValue;
    auto dXt = dXtFunctor(query.simulationParameters.solver);
    std::mt19937 generator(seq);
    Path path = {};
    Utils::assertTrue(points != 0, "Expected points to be non-zero");
    path.reserve(points);
    path.push_back(startValue);
    for (size_t i = 1; i < points; ++i) {
        if (stopToken.stop_requested()) [[unlikely]] return {};
        path.push_back(path.back() + dXt(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt, generator));
    }
    return path;
}

static Paths samplePaths(
        const PathQuery& query,
        EngineThreadPool& threadpool,
        std::stop_token stopToken){
    const size_t nrPathsToDraw = query.simulationParameters.nrPathsToDraw();
    Paths paths{};
    paths.reserve(nrPathsToDraw);
    const uint32_t baseSeed = query.settingsParameters.seed
        ? uint32_t(query.settingsParameters.seed.value())
        : std::random_device{}();
    if (query.settingsParameters.useThreading){
        std::vector<std::future<Path>> pathFutures;
        pathFutures.reserve(nrPathsToDraw);
        for (uint32_t i = 0; i < nrPathsToDraw; ++i) {
            if (stopToken.stop_requested()) break;
            pathFutures.push_back(threadpool.enqueue([query, uniqueSeed = baseSeed + i, stopToken]() mutable {
                return samplePath(query, std::seed_seq{ uniqueSeed }, stopToken);
            }));
        }
        for (auto& future : pathFutures) {
            paths.push_back(future.get());
        }
    } else {
        for (uint32_t i = 0; i < nrPathsToDraw; ++i) {
            paths.push_back(samplePath(query, std::seed_seq{ baseSeed + i }, stopToken));
        }
    }
    return paths;
}

static std::optional<State> samplePathEndValue(
        const PathQuery& query,
        std::seed_seq seq,
        std::stop_token stopToken,
        std::shared_ptr<Job::Atomics> jobData) {
    // Worker threads should always start by checking for user cancellation
    if (stopToken.stop_requested()){
        jobData->pathsCompleted.fetch_add(1, std::memory_order_relaxed);
        return std::nullopt;
    }
    const auto points = query.simulationParameters.points();
    const auto& drift = query.processDefinition.drift;
    const auto& diffusion = query.processDefinition.diffusion;
    const Time dt = query.simulationParameters.dt;
    State X = query.processDefinition.startValue;
    auto dXt = dXtFunctor(query.simulationParameters.solver);
    std::mt19937 generator(seq);
    Utils::assertTrue(points != 0, "Expected points to be non-zero");
    for (size_t i = 1; i < points; ++i) {
        if (stopToken.stop_requested()) [[unlikely]] break;
        X += dXt(drift, diffusion, static_cast<Time>(i) * dt, X, dt, generator);
    }
    jobData->pathsCompleted.fetch_add(1, std::memory_order_relaxed);
    return stopToken.stop_requested() ? std::nullopt : std::make_optional(X);
}

static Distribution sampleDistribution(
        const PathQuery& query,
        EngineThreadPool& threadpool,
        std::shared_ptr<Job::Atomics> jobData,
        std::stop_token stopToken){
    const size_t points = query.simulationParameters.samples;
    Distribution distribution{};
    distribution.reserve(points);
    const uint32_t baseSeed = query.settingsParameters.seed
        ? uint32_t(query.settingsParameters.seed.value())
        : std::random_device{}();
    if (query.settingsParameters.useThreading){
        std::vector<std::future<std::optional<State>>> distributionFuture;
        distributionFuture.reserve(points);
        for (uint32_t i = 0; i < points; ++i) {
            if (stopToken.stop_requested()) break;
            distributionFuture.push_back(threadpool.enqueue([query, uniqueSeed = baseSeed + i, stopToken, jobData, i]() mutable -> std::optional<State> {
                return samplePathEndValue(query, std::seed_seq{ uniqueSeed }, stopToken, jobData);
            }));
        }
        for (auto& future : distributionFuture) {
            if (auto endValue = future.get()) {
                distribution.push_back(endValue.value());
            }
        }
    } else {
        for (uint32_t i = 0; i < points; ++i) {
            if (auto endValue = samplePathEndValue(query, std::seed_seq{ baseSeed + i }, stopToken, jobData)) {
                distribution.push_back(endValue.value());
            }
        }
    }
    return distribution;
}

[[nodiscard]] Job PathEngine::processPathQuery(const PathQuery& pathQuery) {
    Job job{pathQuery.simulationParameters.samples};
    job.fullPaths = std::move(std::async(std::launch::async, samplePaths,
        pathQuery, std::ref(*m_tp), job.stop.get_token()));
    job.distribution = std::move(std::async(std::launch::async, sampleDistribution,
        pathQuery, std::ref(*m_tp), job.atomicData, job.stop.get_token()));
    return job;
}

bool PathEngine::isBusy(){
    return m_tp->nrBusyThreads() != 0;
}