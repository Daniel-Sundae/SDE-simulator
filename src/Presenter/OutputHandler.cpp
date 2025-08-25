#include "OutputHandler.hpp"
#include "OutputDispatcher.hpp"
#include "PathQuery.hpp"
#include "PDF.hpp"
#include "Constants.hpp"
#include "Utils.hpp"

void OutputHandler::signalReadyIfNeeded() const {
    if (m_pathsReceived && m_distributionReceived){
        m_outputDispatcher->setReady();
    }
}

void OutputHandler::onPathsReceived(const Paths& paths){
    Utils::assertTrue(paths.size() <= DefaultConstants::maxPathsToDraw,
        "Expected paths to be at most {} but got {}", DefaultConstants::maxPathsToDraw, paths.size());
    for (const auto& path : paths) {
        m_outputDispatcher->plotPath(path);
    }
    m_pathsReceived = true; 
    signalReadyIfNeeded();
}

void OutputHandler::onDistributionReceived(const Distribution& distribution, const Range support){
    Utils::assertTrue(!m_currentPDF.data.has_value(), "Data has incorrectly already been generated");
    Utils::assertTrue(m_currentPDF.pdf != nullptr, "PDF has not been created");
    auto GUIsupport = std::pair{support.first - m_currentPDF.stddev, support.second + m_currentPDF.stddev};
    m_currentPDF.generatePDFData(GUIsupport);
    m_outputDispatcher->setDistributionChartSupport(GUIsupport);
    m_outputDispatcher->updateDistributionChartPDF(m_currentPDF.data.value());
    m_outputDispatcher->updateDistributionChartEV(m_currentPDF.EV);
    m_outputDispatcher->plotDistribution(distribution);
    m_distributionReceived = true;
    signalReadyIfNeeded();
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

void OutputHandler::onStartTransaction(const PathQuery& query){
    prepareGUI(query);
    m_currentPDF = getField(FieldTags::pdf{},
        query.processDefinition.type,
        query.processDefinition.startValue,
        query.simulationParameters.time,
        query.processDefinition.drift.mu(),
        query.processDefinition.diffusion.sigma()
    );
    m_pathsReceived = false;
    m_distributionReceived = false;
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
}