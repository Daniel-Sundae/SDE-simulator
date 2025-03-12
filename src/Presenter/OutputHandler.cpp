#include "OutputHandler.hpp"
#include "OutputManager.hpp"
#include "PathQuery.hpp"

OutputHandler::OutputHandler()
	: IPresenterComponent()
	, m_distribution{}
{ }

auto OutputHandler::OnPathsReceived(const PathQuery& pQuery, const Paths& paths) -> void
{
	(void)pQuery;
	(void)paths;
	// Listener()->ClearPathChart();
	// Listener()->UpdatePathChartTitle(pQuery);
	// Listener()->UpdateDistributionChartTitle(pQuery.processDefinition.type);
	// Distribution distribution;
	// distribution.reserve(paths.size());
	// for (const auto& p : paths) {
	// 	Listener()->PlotPath(p);
	// 	distribution.push_back(p.back());
	// }
	// m_distribution = std::move(distribution);
}

auto OutputHandler::OnDriftDataReceived(const Path& driftData) const -> void
{
	Listener()->PlotPathChartDriftData(driftData);
}

auto OutputHandler::OnPDFReceived(const PDF& pdf) const -> void
{
	(void)pdf;
	// Listener()->ClearDistributionChart();
	// Listener()->SetXAxisRange(pdf.GetSupport());
	// Listener()->PlotPDF(pdf.GetPDFData());
	// Listener()->PlotEV(pdf.EV());
	// Listener()->PlotDistribution(m_distribution); // Paths must have been sampled first
}

auto OutputHandler::OnClear() const -> void
{
	Listener()->ClearPathChart();
	Listener()->ClearDistributionChart();
}