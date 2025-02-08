#pragma once
#include "Types.hpp"
class PathEngine final : public IModelComponent {
public:
	explicit PathEngine();
	Path SamplePath(void* requester, Query q);
	std::vector<Path> SamplePaths(void* requester, Query q);
private:


}
