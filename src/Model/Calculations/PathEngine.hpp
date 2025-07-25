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
    [[nodiscard]] std::future<Paths> samplePathsAsync(const PathQuery& pQuery);
    void requestCancel();
    bool isBusy();

private:
    std::function<Path()> createPathSamplingFunction(const PathQuery& pathQuery, uint32_t seedId) const;
    template <typename F>
    Path sampleOnePathImpl(const PathQuery &pathQuery, std::mt19937 &generator, F dXt) const;
    State increment(const Drift &drift,
        const Diffusion &diffusion,
        const Time t,
        const State Xt,
        const Time dt,
        std::mt19937 &generator) const;

private:
    std::unique_ptr<EngineThreadPool> m_tp =
        std::make_unique<EngineThreadPool>();
    Paths m_pathResults{};
    std::mutex m_pathsMtx;
    std::atomic<bool> m_cancelRequested{false};
    std::atomic<size_t> m_completedTasks{0};
    std::condition_variable m_completionCv;
    std::mutex m_completionMtx;
    SettingsParameters m_engineSettings{};
};
