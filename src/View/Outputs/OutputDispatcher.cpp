#include "PathChart.hpp"
#include "DistributionChart.hpp"
#include "OutputDispatcher.hpp"
#include "StatusManager.hpp"
#include <QtWidgets/QGraphicsLayout>

OutputDispatcher::OutputDispatcher(QWidget *parent)
    : QWidget(parent)
    , m_statusManager(new StatusManager(this))
    , m_pathChartView(new QChartView(this))
    , m_distributionChartView(new QChartView(this))
{
    layout = new QVBoxLayout(this);
    layout->addWidget(m_statusManager);
    // Wrapper is only for aligning the Status Manager with the charts
    QVBoxLayout* groupBoxWrapperLayout = new QVBoxLayout();
    groupBoxWrapperLayout->setContentsMargins(8, 0, 8, 0);
    groupBoxWrapperLayout->addWidget(m_statusManager);
    layout->addLayout(groupBoxWrapperLayout);
    auto* pathChart = new PathChart();
    m_pathChartView->setChart(pathChart);
    layout->addWidget(m_pathChartView);
    auto* distributionChart = new DistributionChart();
    m_distributionChartView->setChart(distributionChart);
    layout->addWidget(m_distributionChartView);
    setLayout(layout);
}

////////////////////////////////////////
//// STATUS MANAGER
////////////////////////////////////////

void OutputDispatcher::setQueryInfo(const PathQuery& pQuery) const{
    m_statusManager->setQueryInfo(pQuery);
}

void OutputDispatcher::setStatus(const StatusSignal signal) const{
    m_statusManager->setStatus(signal);
}

void OutputDispatcher::clearStatus() const{
    m_statusManager->clearStatus();
}

////////////////////////////////////////
//// PATH CHART
////////////////////////////////////////

void OutputDispatcher::updatePathChartTitle(bool allPathsPlotted) const{
    getPathChart()->updateTitle(allPathsPlotted);
}

void OutputDispatcher::plotPathChartDriftData(const Path& driftLine) const{
    getPathChart()->plotDriftCurve(driftLine);
}

void OutputDispatcher::plotPath(const Path& path) const{
    getPathChart()->plotPath(path);
}

void OutputDispatcher::clearPathChart(bool clearDrift){
    getPathChart()->clearPathChart(clearDrift);
}

void OutputDispatcher::setPathChartMaxTime(const Time time){
    getPathChart()->setMaxTime(time);
}

////////////////////////////////////////
//// DISTRIBUTION CHART
////////////////////////////////////////

void OutputDispatcher::updateDistributionChartTitle(const ProcessType type) const{
    getDistributionChart()->updateTitle(type);
}

void OutputDispatcher::plotDistribution(const Distribution& distribution) const{
    getDistributionChart()->plotDistribution(distribution);
}

void OutputDispatcher::updateDistributionChartPDF(const PDFData& pdfData) const{
    getDistributionChart()->updateDistributionChartPDF(pdfData);
}

void OutputDispatcher::setDistributionChartSupport(const Range pdfDomain) const{
    getDistributionChart()->setDistributionChartSupport(pdfDomain);
}

void OutputDispatcher::updateDistributionChartEV(const State EV) const{
    getDistributionChart()->plotExpValLine(EV);
}

void OutputDispatcher::clearDistributionChart(){
    getDistributionChart()->clearDistributionChart();
}

////////////////////////////////////////
//// ACCESSORS
////////////////////////////////////////

PathChart* OutputDispatcher::getPathChart() const{
    return static_cast<PathChart*>(m_pathChartView->chart());
}

DistributionChart* OutputDispatcher::getDistributionChart() const{
    return static_cast<DistributionChart*>(m_distributionChartView->chart());
}