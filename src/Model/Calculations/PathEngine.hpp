#pragma once
#include "Types.hpp"
#include "EngineThreadPool.hpp"
#include <memory>

struct PathQuery;
struct PDFQuery;

class PathEngine{
public:
	explicit PathEngine();
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
	std::unique_ptr<EngineThreadPool> m_tp;
};
