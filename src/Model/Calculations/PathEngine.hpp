#pragma once
#include "Types.hpp"
#include "EngineThreadPool.hpp"
#include <memory>
#include <mutex>
#include <optional>
#include <functional>

struct PathQuery;
struct PDFQuery;

class PathEngine{
public:
	explicit PathEngine();
	auto SamplePathsAsync(const PathQuery& pathQuery, std::function<void(Paths)> onCompletionCb) -> void;
	auto SampleDriftCurve(const PathQuery& pathQuery) const -> Path;
	auto GeneratePDFData(const PDFQuery& pdfQuery) const -> PDFData;
	auto RequestCancel() -> void;
    auto IsBusy() -> bool;
private:
    auto SamplePathGenerator(const PathQuery& pathQuery, const std::size_t slot) -> std::function<void()>;
	inline auto Increment(
		const Drift& drift,
		const Diffusion& diffusion,
		const Time t,
		const State Xt,
		const Time dt) const -> State;
	std::unique_ptr<EngineThreadPool> m_tp;
	Paths m_paths;
	std::mutex m_pathsMtx;
	std::atomic<bool> m_cancelRequested;
	std::atomic<std::size_t> m_completedTasks;
	std::condition_variable m_completionCv;
	std::mutex m_completionMtx;
	std::atomic<bool> m_isBusy;
};
