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
struct PDFQuery;

class PathEngine{
public:
	explicit PathEngine();
    auto SamplePathsAsync(const PathQuery &pathQuery, std::function<void(Paths)> onCompletionCb) -> void;
    auto SampleOnePath(const PathQuery& pathQuery) const -> Path;
    auto RequestCancel() -> void;
    auto IsBusy() -> bool;

private:
    auto SamplePathFunctor(const PathQuery &pathQuery, const std::size_t slot, const std::size_t seed) -> std::function<void()>;
    auto SampleOnePathImpl(const PathQuery &pathQuery, std::mt19937 &generator) const -> Path;
    auto Increment(
		const Drift &drift,
		const Diffusion &diffusion,
		const Time t,
		const State Xt,
		const Time dt,
		std::mt19937 &generator) const -> State;

private:
	std::unique_ptr<EngineThreadPool> m_tp;
	Paths m_pathResults;
	std::mutex m_pathsMtx;
	std::atomic<bool> m_cancelRequested;
	std::atomic<std::size_t> m_completedTasks;
	std::condition_variable m_completionCv;
	std::mutex m_completionMtx;
	SettingsParameters m_engineSettings;
};
