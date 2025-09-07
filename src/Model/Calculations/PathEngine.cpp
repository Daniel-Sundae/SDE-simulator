#include "PathEngine.hpp"
#include "PathQuery.hpp"
#include "SolverData.hpp"
#include <thread>

static void updateJobData(
        std::shared_ptr<Job::MetaData> jobData,
        const State XT,
        const State minXt,
        const State maxXt
    ) {
    jobData->pathsCompleted.fetch_add(1, std::memory_order_relaxed);
    State currentMinXT = jobData->minXT.load();
    State currentMaxXT = jobData->maxXT.load();
    State currentMinXt = jobData->minXt.load();
    State currentMaxXt = jobData->maxXt.load();
    // If current value is stale, update it and if needed, try again
    while(XT < currentMinXT && !jobData->minXT.compare_exchange_strong(currentMinXT, XT, std::memory_order_relaxed, std::memory_order_relaxed));
    while(XT > currentMaxXT && !jobData->maxXT.compare_exchange_strong(currentMaxXT, XT, std::memory_order_relaxed, std::memory_order_relaxed));
    while(minXt < currentMinXt && !jobData->minXt.compare_exchange_strong(currentMinXt, minXt, std::memory_order_relaxed, std::memory_order_relaxed));
    while(maxXt > currentMaxXt && !jobData->maxXt.compare_exchange_strong(currentMaxXt, maxXt, std::memory_order_relaxed, std::memory_order_relaxed));
}

static Path samplePath(
        const PathQuery& query,
        std::seed_seq seq,
        std::shared_ptr<Job::MetaData> jobData,
        std::stop_token stopToken) {
    if (stopToken.stop_requested()) return {};
    const auto points = query.simulationParameters.steps() + 1;
    const auto& drift = query.definitionParameters.drift;
    const auto& diffusion = query.definitionParameters.diffusion;
    const Time dt = query.simulationParameters.dt;
    State XT = query.definitionParameters.X0;
    State minXt = XT;
    State maxXt = XT;
    auto dXt = dXtFunction(query.simulationParameters.solver);
    std::mt19937 generator(seq);
    std::normal_distribution<double> noise(0.0, 1.0);
    Path path = {};
    Utils::assertTrue(points >= 1, "Expected points to be at least 1");
    path.reserve(points);
    path.push_back(XT);
    for (size_t i = 1; i < points; ++i) {
        if (stopToken.stop_requested()) [[unlikely]] return {};
        State dB = noise(generator) * std::sqrt(dt);
        Time t = static_cast<Time>(i) * dt;
        XT += dXt(drift, diffusion, t, XT, dt, dB);
        path.push_back(XT);
        minXt = std::min(minXt, XT);
        maxXt = std::max(maxXt, XT);
    }
    updateJobData(jobData, XT, minXt, maxXt);
    return path;
}

static Paths samplePaths(
        const StochasticFullPathsQuery& query,
        EngineThreadPool& threadpool,
        std::shared_ptr<Job::MetaData> jobData,
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
            pathFutures.push_back(threadpool.enqueue([query, uniqueSeed = baseSeed + i, stopToken, jobData]() mutable {
                return samplePath(query, std::seed_seq{ uniqueSeed }, jobData, stopToken);
            }));
        }
        for (auto& future : pathFutures) {
            paths.push_back(future.get());
        }
    } else {
        for (uint32_t i = 0; i < nrPathsToDraw; ++i) {
            paths.push_back(samplePath(query, std::seed_seq{ baseSeed + i }, jobData, stopToken));
        }
    }
    return paths;
}

static std::optional<State> samplePathXT(
        const PathQuery& query,
        std::seed_seq seq,
        std::shared_ptr<Job::MetaData> jobData,
        std::stop_token stopToken) {
    // Worker threads should always start by checking for user cancellation
    if (stopToken.stop_requested()) return std::nullopt;
    const auto points = query.simulationParameters.steps() + 1;
    const auto& drift = query.definitionParameters.drift;
    const auto& diffusion = query.definitionParameters.diffusion;
    const Time dt = query.simulationParameters.dt;
    State XT = query.definitionParameters.X0;
    State minXt = XT;
    State maxXt = XT;
    auto dXt = dXtFunction(query.simulationParameters.solver);
    std::mt19937 generator(seq);
    std::normal_distribution<double> noise(0.0, 1.0);
    Utils::assertTrue(points >= 1, "Expected points to be at least 1");
    for (size_t i = 1; i < points; ++i) {
        if (stopToken.stop_requested()) [[unlikely]] return std::nullopt;
        State dB = noise(generator) * std::sqrt(dt);
        Time t = static_cast<Time>(i) * dt;
        XT += dXt(drift, diffusion, t, XT, dt, dB);
        minXt = std::min(minXt, XT);
        maxXt = std::max(maxXt, XT);
    }
    updateJobData(jobData, XT, minXt, maxXt);
    return std::make_optional(XT);
}

static Distribution sampleDistribution(
        const PathQuery& query,
        EngineThreadPool& threadpool,
        std::shared_ptr<Job::MetaData> jobData,
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
                return samplePathXT(query, std::seed_seq{ uniqueSeed }, jobData, stopToken);
            }));
        }
        for (auto& future : distributionFuture) {
            if (auto XT = future.get()) {
                distribution.push_back(XT.value());
            }
        }
    } else {
        for (uint32_t i = 0; i < points; ++i) {
            if (auto XT = samplePathXT(query, std::seed_seq{ baseSeed + i }, jobData, stopToken)) {
                distribution.push_back(XT.value());
            }
        }
    }
    return distribution;
}

[[nodiscard]] AnyJob PathEngine::processQuery(const AnyQuery& aQuery) {
    return std::visit([this](auto const& query) -> AnyJob {
        using QueryType = std::decay_t<decltype(query)>;
        if constexpr (std::is_same_v<QueryType, DeterministicQuery>){
            DeterministicJob dJob(query.simulationParameters.samples);
            dJob.drift = std::move(std::async(std::launch::async,
                [query, md = dJob.metaData, st = dJob.stop.get_token()]() mutable {
                return samplePath(query, std::seed_seq{0}, md, st);
            }));
            return dJob;
        } else if constexpr (std::is_same_v<QueryType, StochasticQuery>) {
            StochasticJob sJob(query.simulationParameters.samples);
            sJob.distribution = std::move(std::async(std::launch::async, sampleDistribution,
                query, std::ref(*m_tp), sJob.metaData, sJob.stop.get_token()));
            return sJob;
        } else if constexpr (std::is_same_v<QueryType, StochasticFullPathsQuery>) {
            StochasticFullPathsJob fpJob(query.simulationParameters.samples);
            fpJob.fullPaths = std::move(std::async(std::launch::async, samplePaths,
                query, std::ref(*m_tp), fpJob.metaData, fpJob.stop.get_token()));
            return fpJob;
        }
    }, aQuery);
}