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

class PathEngine{
public:
    explicit PathEngine();
    void samplePathsAsync(const PathQuery &pathQuery, std::function<void(Paths)> onCompletionCb);
    Path sampleOnePath(const PathQuery& pathQuery) const;
    void requestcancel();
    bool isBusy();

private:
    std::function<void()> samplePathFunctor(const PathQuery &pathQuery, const size_t slot, const size_t seed);
    template <typename F>Path sampleOnePathImpl(const PathQuery &pathQuery, std::mt19937 &generator, F dXt) const;
    State increment(const Drift &drift,
        const Diffusion &diffusion,
        const Time t,
        const State Xt,
        const Time dt,
        std::mt19937 &generator) const;

private:
    std::unique_ptr<EngineThreadPool> m_tp;
    Paths m_pathResults;
    std::mutex m_pathsMtx;
    std::atomic<bool> m_cancelRequested;
    std::atomic<size_t> m_completedTasks;
    std::condition_variable m_completionCv;
    std::mutex m_completionMtx;
    SettingsParameters m_engineSettings;
};
