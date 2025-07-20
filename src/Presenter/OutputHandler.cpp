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
    for (auto& path : paths) {
        listener()->plotPath(path);
        distribution.push_back(path.back());
    }
    listener()->plotDistribution(distribution);
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

void OutputHandler::onPDFReceived(PDF&& pdf){
    listener()->setDistributionChartSupport(pdf.getSupport());
    listener()->updateDistributionChartPDF(pdf.getPDFData());
    listener()->updateDistributionChartEV(pdf.EV());
}

void OutputHandler::clear() const{
    listener()->clearStatus();
    listener()->clearPathChart();
    listener()->clearDistributionChart();
}