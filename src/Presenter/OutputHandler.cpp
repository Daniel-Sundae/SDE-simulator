#include "OutputHandler.hpp"
#include "OutputDispatcher.hpp"
#include "PathQuery.hpp"
#include "PDF.hpp"
#include <QtCore/qmetatype.h> // For QMetaType registration (needed for connect)

OutputHandler::OutputHandler(QObject* parent)
	: QObject(parent) // Initialize QObject base class
	, IPresenterComponent()
	, m_distributionSupport(std::make_pair<State>(0,0))
	, m_paths{}
	, m_driftCurve{}
{
    connect(this, &OutputHandler::internalPathsReady,
            this, &OutputHandler::OnPathsReceived,
            Qt::QueuedConnection);
}

auto OutputHandler::HandleWorkerResult(const PathQuery& query, Paths paths) -> void
{
    emit internalPathsReady(query, std::move(paths));
}

auto OutputHandler::OnPathsReceived(const PathQuery& pQuery, Paths paths) -> void
{
	m_paths = std::move(paths);
	// Change statustext from "Generating paths" to "Rendering GUI"
	Listener()->ClearPathChart(false);
	Listener()->SetPathChartMaxTime(pQuery.simulationParameters.time);
	Listener()->UpdatePathChartTitle(pQuery);
	Listener()->UpdateDistributionChartTitle(pQuery.processDefinition.type);
	Distribution distribution;
	distribution.reserve(m_paths.size());
	for (std::size_t i = 0; i < m_paths.size(); ++i) {
		const Path& p = m_paths[i];
		if(i < 20){
			Listener()->PlotPath(p);
		}
		if(IsInSupport(p.back())) {
			distribution.push_back(p.back());
		}
	}
	Listener()->PlotDistribution(distribution);
	// Remove loading symbol if there currently is one
}

auto OutputHandler::OnDriftDataReceived(Path&& driftCurve) -> void
{
	m_driftCurve = std::move(driftCurve);
	Listener()->PlotPathChartDriftData(driftCurve);
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
