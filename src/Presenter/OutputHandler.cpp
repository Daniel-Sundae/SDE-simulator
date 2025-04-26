#include "OutputHandler.hpp"
#include "OutputDispatcher.hpp"
#include "PathQuery.hpp"
#include "PDF.hpp"
#include "Constants.hpp"
#include <QtCore/QMetaType> // For QMetaType registration (needed for connect)

OutputHandler::OutputHandler(QObject* parent)
	: QObject(parent) // Initialize QObject base class
	, IPresenterComponent()
	, m_distributionSupport(std::make_pair<State>(0,0))
{
    connect(this, &OutputHandler::InternalPathReadySignal,
            this, &OutputHandler::OnPathsReceived,
            Qt::QueuedConnection);
}

auto OutputHandler::HandleWorkerResult(Paths&& paths) -> void
{
	// Pass data ownership from worker thread to GUI thread.
    emit InternalPathReadySignal(std::move(paths));
}

auto OutputHandler::OnPathsReceived(const Paths& paths) -> void
{
	Listener()->SetStatus(StatusSignal::RENDERING);
	Distribution distribution;
	distribution.reserve(paths.size());
	assert(HasSupport());
	for (std::size_t i = 0; i < paths.size(); ++i) {
		const Path& p = paths[i];
		if(i < DefaultConstants::maxPathsToDraw){
			Listener()->PlotPath(p);
		}
		if(IsInSupport(p.back())) {
			distribution.push_back(p.back());
		}
	}
	Listener()->PlotDistribution(distribution);
	DeleteSupport();
	Listener()->SetStatus(StatusSignal::READY);
}

auto OutputHandler::OnDriftDataReceived(Path&& driftCurve) -> void
{
	Listener()->PlotPathChartDriftData(driftCurve);
}

auto OutputHandler::PrepareGUI(const PathQuery& pQuery) -> void
{
	Clear();
	Listener()->SetStatus(StatusSignal::SAMPLING);
	Listener()->SetQueryInfo(pQuery);
	Listener()->ClearPathChart(false);
	Listener()->SetPathChartMaxTime(pQuery.simulationParameters.time);
	Listener()->UpdateDistributionChartTitle(pQuery.processDefinition.type);
	Listener()->UpdatePathChartTitle(DefaultConstants::maxPathsToDraw >= pQuery.simulationParameters.samples);
}

auto OutputHandler::OnPDFReceived(const PDF& pdf) -> void
{
	m_distributionSupport = pdf.GetSupport();
	Listener()->SetDistributionChartSupport(m_distributionSupport);
	Listener()->UpdateDistributionChartPDF(pdf.GetPDFData());
	Listener()->UpdateDistributionChartEV(pdf.EV());
}

auto OutputHandler::Clear() const -> void
{
	Listener()->ClearStatus();
	Listener()->ClearPathChart();
	Listener()->ClearDistributionChart();
}

auto OutputHandler::HasSupport() const -> bool
{
	return m_distributionSupport.first && m_distributionSupport.second ? true : false;
}

auto OutputHandler::DeleteSupport() -> void
{
	m_distributionSupport.first = 0;
	m_distributionSupport.second = 0;
}

auto OutputHandler::IsInSupport(const State s) const -> bool
{
	return m_distributionSupport.first < s && s < m_distributionSupport.second ? true : false;
}
