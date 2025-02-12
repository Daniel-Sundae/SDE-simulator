#pragma once
#include "Types.hpp"
class PathEngine final : public IModelComponent {
public:
	explicit PathEngine();
	auto SamplePath(const PathQuery& pathQuery) const -> Path;
	auto SamplePaths(const PathQuery& pathQuery, std::size_t samples) const -> std::vector<Path>;
private:
	inline auto Increment(Drift drift, Diffusion diffusion, Time t, State Xt, Time dt) const -> State;


}
