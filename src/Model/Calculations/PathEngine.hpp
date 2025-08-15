#pragma once
#include "Types.hpp"
#include "EngineThreadPool.hpp"
#include <memory>
#include <mutex>
#include <optional>
#include <functional>
#include <random>
#include <cmath>

struct PathQuery;
struct Job {
    std::future<Paths> result;
    std::stop_source stop;
    struct Progress {
        size_t totalTasks = 0;
        std::atomic<size_t> completedTasks = 0;
    } progress;
    bool isCancelled() const { return stop.stop_requested(); }
    bool doCancel()  { return stop.request_stop(); }
    bool isDone() const { return progress.completedTasks == progress.totalTasks; }
};
class PathEngine{
public:
    explicit PathEngine() = default;
    [[nodiscard]] Job samplePathsAsync(const PathQuery& pQuery);
    void requestCancel();
    bool isBusy();

private:
    State increment(const Drift &drift,
        const Diffusion &diffusion,
        const Time t,
        const State Xt,
        const Time dt,
        std::mt19937 &generator) const;

private:
    std::unique_ptr<EngineThreadPool> m_tp =
        std::make_unique<EngineThreadPool>();
    std::atomic<bool> m_cancelRequested{false};
};
