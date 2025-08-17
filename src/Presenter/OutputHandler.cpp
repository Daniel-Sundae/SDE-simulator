#include "OutputHandler.hpp"
#include "OutputDispatcher.hpp"
#include "PathQuery.hpp"
#include "PDF.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include <QtCore/QMetaType> // For QMetaType registration (needed for connect)

void OutputHandler::onPathsReceived(const Paths& paths){
    m_outputDispatcher->setStatus(StatusSignal::RENDERING);
    Distribution distribution;
    distribution.reserve(paths.size());
    for (const auto& path : paths) {
        m_outputDispatcher->plotPath(path);
        distribution.push_back(path.back());
    }
    m_outputDispatcher->plotDistribution(distribution);
    m_outputDispatcher->setStatus(StatusSignal::READY);
}

void OutputHandler::onDriftDataReceived(const Path& driftCurve){
    m_outputDispatcher->plotPathChartDriftData(driftCurve);
}

void OutputHandler::onPDFReceived(const PDF& pdf){
    m_outputDispatcher->setDistributionChartSupport(pdf.getSupport());
    m_outputDispatcher->updateDistributionChartPDF(pdf.getPDFData());
    m_outputDispatcher->updateDistributionChartEV(pdf.EV());
}

void OutputHandler::prepareGUI(const PathQuery& pQuery){
    clear();
    m_outputDispatcher->setStatus(StatusSignal::SAMPLING);
    m_outputDispatcher->setQueryInfo(pQuery);
    m_outputDispatcher->clearPathChart(false);
    m_outputDispatcher->setPathChartMaxTime(pQuery.simulationParameters.time);
    m_outputDispatcher->updateDistributionChartTitle(pQuery.processDefinition.type);
    m_outputDispatcher->updatePathChartTitle(DefaultConstants::maxPathsToDraw >= pQuery.simulationParameters.samples);
}

void OutputHandler::clear() const{
    m_outputDispatcher->clearStatus();
    m_outputDispatcher->clearPathChart();
    m_outputDispatcher->clearDistributionChart();
}