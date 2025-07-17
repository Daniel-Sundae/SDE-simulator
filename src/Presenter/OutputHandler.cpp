#include "OutputHandler.hpp"
#include "OutputDispatcher.hpp"
#include "PathQuery.hpp"
#include "PDF.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include <QtCore/QMetaType> // For QMetaType registration (needed for connect)

OutputHandler::OutputHandler(QObject* parent)
    : QObject(parent) // Initialize QObject base class
    , IPresenterComponent()
    , m_distributionSupport(std::make_pair<State>(0,0))
{
    connect(this, &OutputHandler::internalPathReadySignal,
            this, &OutputHandler::onPathsReceived,
            Qt::QueuedConnection);
}

void OutputHandler::handleWorkerResult(Paths&& paths){
    // Pass data ownership from worker thread to GUI thread.
    emit internalPathReadySignal(std::move(paths));
}

void OutputHandler::onPathsReceived(const Paths& paths){
    listener()->setStatus(StatusSignal::RENDERING);
    Distribution distribution;
    distribution.reserve(paths.size());
    if(!hasSupport()){
        fatalError("No support defined for distribution chart. m_distributionSupport: ({}, {})",
                   m_distributionSupport.first, m_distributionSupport.second);
    }
    for (size_t i = 0; i < paths.size(); ++i) {
        const Path& p = paths[i];
        if(i < DefaultConstants::maxPathsToDraw){
            listener()->plotPath(p);
        }
        if(isInSupport(p.back())) {
            distribution.push_back(p.back());
        }
    }
    listener()->plotDistribution(distribution);
    deleteSupport();
    listener()->setStatus(StatusSignal::READY);
}

void OutputHandler::onDriftDataReceived(Path&& driftCurve){
    listener()->plotPathChartDriftData(driftCurve);
}

void OutputHandler::prepareGUI(const PathQuery& pQuery){
    clear();
    listener()->setStatus(StatusSignal::SAMPLING);
    listener()->setQueryInfo(pQuery);
    listener()->clearPathChart(false);
    listener()->setPathChartMaxTime(pQuery.simulationParameters.time);
    listener()->updateDistributionChartTitle(pQuery.processDefinition.type);
    listener()->updatePathChartTitle(DefaultConstants::maxPathsToDraw >= pQuery.simulationParameters.samples);
}

void OutputHandler::onPDFReceived(const PDF& pdf){
    m_distributionSupport = pdf.getSupport();
    listener()->setDistributionChartSupport(m_distributionSupport);
    listener()->updateDistributionChartPDF(pdf.getPDFData());
    listener()->updateDistributionChartEV(pdf.EV());
}

void OutputHandler::clear() const{
    listener()->clearStatus();
    listener()->clearPathChart();
    listener()->clearDistributionChart();
}

bool OutputHandler::hasSupport() const{
    return m_distributionSupport.first && m_distributionSupport.second ? true : false;
}

void OutputHandler::deleteSupport(){
    m_distributionSupport.first = 0;
    m_distributionSupport.second = 0;
}

bool OutputHandler::isInSupport(const State s) const{
    return m_distributionSupport.first < s && s < m_distributionSupport.second ? true : false;
}
