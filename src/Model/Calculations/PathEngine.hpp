#pragma once
#include "Types.hpp"

struct PathQuery;
struct PDFQuery;

class PathEngine{
public:
	auto SamplePaths(const PathQuery& pQuery) const -> Paths;
	auto SamplePDF(const PDFQuery& pdfQuery) const -> PDFData;
private:
	inline auto Increment(
		const Drift& drift,
		const Diffusion& diffusion,
		const Time t,
		const State Xt,
		const Time dt) const -> State;
};
