#include "OutputHandler.hpp"
#include "OutputManager.hpp"

OutputHandler::OutputHandler()
	: IPresenterComponent()
{ }

auto OutputHandler::OnPathReceived(const PathQuery& pQuery, const Path& path) const -> void
{
	Listener()->PlotPath(path);
	Listener()->UpdateChartTitle(pQuery);
}
auto OutputHandler::OnDriftLineReceived(const Path& driftLine) const -> void
{
	Listener()->PlotDriftLine(driftLine);
}

auto OutputHandler::OnClear() const -> void
{
	Listener()->Clear();
}
