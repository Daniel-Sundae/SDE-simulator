#include "OutputHandler.hpp"
#include "OutputDispatcher.hpp"
#include "PathQuery.hpp"
#include "PDF.hpp"

OutputHandler::OutputHandler()
	: IPresenterComponent()
	, m_distributionSupport(std::make_pair<State>(0,0))
{ }

auto OutputHandler::OnPathsReceived(const PathQuery& pQuery, const Paths& paths) -> void
{
	Listener()->ClearPathChart(false);
	Listener()->SetPathChartMaxTime(pQuery.simulationParameters.time);
	Listener()->UpdatePathChartTitle(pQuery);
	Listener()->UpdateDistributionChartTitle(pQuery.processDefinition.type);
	Distribution distribution;
	distribution.reserve(paths.size());
	for (const auto& p : paths) {
		Listener()->PlotPath(p);
		if(IsInSupport(p.back())) {
			distribution.push_back(p.back());
		}
	}
	Listener()->PlotDistribution(distribution);
	// Remove loading symbol if there currently is one
}

auto OutputHandler::OnDriftDataReceived(const Path& driftData) const -> void
{
	Listener()->PlotPathChartDriftData(driftData);
}

auto OutputHandler::OnPDFReceived(const PDF& pdf) -> void
{
	Listener()->ClearDistributionChart();
	m_distributionSupport = pdf.GetSupport();
	Listener()->SetDistributionChartSupport(m_distributionSupport);
	Listener()->UpdateDistributionChartPDF(pdf.GetPDFData().value());
	Listener()->UpdateDistributionChartEV(pdf.EV());
}

auto OutputHandler::OnClear() -> void
{
	Listener()->ClearPathChart();
	Listener()->ClearDistributionChart();
}

auto OutputHandler::IsInSupport(const State s) const -> bool
{
	return m_distributionSupport.first < s && s < m_distributionSupport.second ? true : false;
}
