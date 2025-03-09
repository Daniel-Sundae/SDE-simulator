#include "OutputHandler.hpp"
#include "OutputManager.hpp"
#include "PathQuery.hpp"

OutputHandler::OutputHandler()
	: IPresenterComponent()
{ }

auto OutputHandler::OnPathsReceived(const PathQuery& pQuery, const Paths& paths) const -> void
{
	Listener()->ClearPaths();
	Listener()->ClearDistribution();
	Listener()->UpdatePathChartTitle(pQuery);
	Listener()->UpdateDistributionChartTitle(pQuery.processDefinition.type);
	std::vector<State> distribution;
	distribution.reserve(paths.size());
	for (const auto& p : paths) {
		Listener()->PlotPath(p);
		distribution.push_back(p.back());
	}
	Listener()->PlotDistribution(distribution);
}

auto OutputHandler::OnDriftDataReceived(const Path& driftData) const -> void
{
	Listener()->PlotPathChartDriftData(driftData);
}

auto OutputHandler::OnPDFDataReceived(const State EV, const PDFData& pdfData) const -> void
{
	Listener()->PlotPDF(pdfData);
	Listener()->PlotEV(EV);
}

auto OutputHandler::OnClear() const -> void
{
	Listener()->ClearPaths();
}