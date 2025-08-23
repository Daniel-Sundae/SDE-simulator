#include "OutputHandler.hpp"
#include "OutputDispatcher.hpp"
#include "PathQuery.hpp"
#include "PDF.hpp"
#include "Constants.hpp"
#include "Utils.hpp"

void OutputHandler::onPathsReceived(const Paths& paths){
    Distribution distribution;
    distribution.reserve(paths.size());
    for (const auto& path : paths) {
        if(path.empty()) break; // Job cancelled
        m_outputDispatcher->plotPath(path);
        distribution.push_back(path.back());
    }
    m_outputDispatcher->plotDistribution(distribution);
    m_outputDispatcher->setReady();
}

void OutputHandler::onDriftDataReceived(const Path& driftCurve){
    m_outputDispatcher->plotPathChartDriftData(driftCurve);
}

void OutputHandler::jobProgress(size_t pathsCompleted){
    m_outputDispatcher->setProgress(pathsCompleted);
}

void OutputHandler::setError(ErrorType error){
    m_outputDispatcher->setError(error);
}

void OutputHandler::onPDFReceived(const PDF& pdf){
    m_outputDispatcher->setDistributionChartSupport(pdf.getSupport());
    m_outputDispatcher->updateDistributionChartPDF(pdf.getPDFData());
    m_outputDispatcher->updateDistributionChartEV(pdf.EV());
}

void OutputHandler::prepareGUI(const PathQuery& query){
    m_outputDispatcher->clearPathChart();
    m_outputDispatcher->clearDistributionChart();
    m_outputDispatcher->prepareStatusInfo(query.simulationParameters.samples);
    m_outputDispatcher->setQueryInfo(query);
    m_outputDispatcher->clearPathChart(false);
    m_outputDispatcher->setPathChartMaxTime(query.simulationParameters.time);
    m_outputDispatcher->updateDistributionChartTitle(query.processDefinition.type);
    m_outputDispatcher->updatePathChartTitle(DefaultConstants::maxPathsToDraw >= query.simulationParameters.samples);
}

void OutputHandler::clearGUI() const{
    m_outputDispatcher->clearStatusBar();
    m_outputDispatcher->clearPathChart();
    m_outputDispatcher->clearDistributionChart();
}

void OutputHandler::cancelGUI() const{
    m_outputDispatcher->cancelStatusBar();
    m_outputDispatcher->clearPathChart();
    m_outputDispatcher->clearDistributionChart();
}