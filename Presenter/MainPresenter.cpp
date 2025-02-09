#include "MainPresenter.hpp"
#include "PathEngine.hpp"

auto MainPresenter::SamplePath(const PathQuery& pathQuery) const -> void
{
	PathEngine engine{};
	Listener()->ShowPath(engine.SamplePath(pathQuery));
}