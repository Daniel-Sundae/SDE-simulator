#pragma once
#include "Types.hpp"

struct PathQuery;
struct PDFQuery;
class ThreadPool;

class PathEngine{
public:
	auto SamplePaths(const PathQuery& pQuery) const -> Paths;
	auto SamplePath(const PathQuery& pathQuery) const -> Path;
	auto GeneratePDFData(const PDFQuery& pdfQuery) const -> PDFData;
	auto Stop() const -> void;
private:
	inline auto Increment(
		const Drift& drift,
		const Diffusion& diffusion,
		const Time t,
		const State Xt,
		const Time dt) const -> State;
	bool m_isBusy;
	std::unique_ptr<ThreadPool> m_tp;
};
