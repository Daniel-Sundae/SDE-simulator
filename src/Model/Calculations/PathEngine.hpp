#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"

class PathEngine{
public:
	auto SamplePaths(const PathQuery& pQuery) const -> Paths;
	auto PDFData(const PDFQuery& pdfQuery) const -> PDF;
private:
	inline auto Increment(
		const Drift& drift,
		const Diffusion& diffusion,
		const Time t,
		const State Xt,
		const Time dt) const -> State;
};
