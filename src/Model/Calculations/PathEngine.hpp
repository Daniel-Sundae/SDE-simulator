#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"

class PathEngine{
public:
	auto SamplePath(const PathQuery& pathQuery) const -> Path;
	auto SamplePaths(const PathQuery& pathQuery, std::size_t samples) const -> std::vector<Path>;
private:
	inline auto Increment(
		const Drift& drift,
		const Diffusion& diffusion,
		const Time t,
		const State Xt,
		const Time dt) const -> State;
};
