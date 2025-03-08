#include "OutputHandler.hpp"
#include "OutputManager.hpp"

OutputHandler::OutputHandler()
	: IPresenterComponent()
{ }

auto OutputHandler::OnPathsReceived(const PathQuery& pQuery, const Paths& paths) const -> void
{
	Listener()->ClearPaths();
	Listener()->ClearDistribution();
	Listener()->UpdatePathChartTitle(pQuery);
	Listener()->UpdateDistributionChartTitle(pQuery);
	std::vector<State> distribution;
	distribution.reserve(paths.size());
	for (const auto& p : paths) {
		Listener()->PlotPath(p);
		distribution.push_back(p.back());
	}
	Listener()->PlotDistribution(distribution);
}
auto OutputHandler::OnDriftLineReceived(const Path& driftLine) const -> void
{
	Listener()->PlotPathChartDriftLine(driftLine);
}

auto OutputHandler::OnClear() const -> void
{
	Listener()->ClearPaths();
}