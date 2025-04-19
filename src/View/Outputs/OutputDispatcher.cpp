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

auto OutputDispatcher::SetQueryInfo(const PathQuery& pQuery) const -> void
{
    m_statusManager->SetQueryInfo(pQuery);
}

auto OutputDispatcher::SetStatus(const StatusSignal signal) const -> void
{
    m_statusManager->SetStatus(signal);
}

////////////////////////////////////////
//// PATH CHART
////////////////////////////////////////

auto OutputDispatcher::UpdatePathChartTitle(bool allPathsPlotted) const -> void
{
    GetPathChart()->UpdateTitle(allPathsPlotted);
}

auto OutputDispatcher::PlotPathChartDriftData(const Path& driftLine) const -> void
{
    GetPathChart()->PlotDriftCurve(driftLine);
}

auto OutputDispatcher::PlotPath(const Path& path) const -> void
{
    GetPathChart()->PlotPath(path);
}

auto OutputDispatcher::ClearPathChart(bool clearDrift) -> void
{
    GetPathChart()->ClearPathChart(clearDrift);
}

auto OutputDispatcher::SetPathChartMaxTime(const Time time) -> void
{
    GetPathChart()->SetMaxTime(time);
}

////////////////////////////////////////
//// DISTRIBUTION CHART
////////////////////////////////////////

auto OutputDispatcher::UpdateDistributionChartTitle(const ProcessType type) const -> void
{
    GetDistributionChart()->UpdateTitle(type);
}

auto OutputDispatcher::PlotDistribution(const Distribution& distribution) const -> void
{
    GetDistributionChart()->PlotDistribution(distribution);
}

auto OutputDispatcher::UpdateDistributionChartPDF(const PDFData& pdfData) const -> void
{
    GetDistributionChart()->UpdateDistributionChartPDF(pdfData);
}

auto OutputDispatcher::SetDistributionChartSupport(const Range pdfDomain) const -> void
{
    GetDistributionChart()->SetDistributionChartSupport(pdfDomain);
}

auto OutputDispatcher::UpdateDistributionChartEV(const State EV) const -> void
{
    GetDistributionChart()->PlotExpValLine(EV);
}

auto OutputDispatcher::ClearDistributionChart() -> void
{
    GetDistributionChart()->ClearDistributionChart();
}

////////////////////////////////////////
//// ACCESSORS
////////////////////////////////////////

auto OutputDispatcher::GetPathChart() const -> PathChart*
{
    return static_cast<PathChart*>(m_pathChartView->chart());
}

auto OutputDispatcher::GetDistributionChart() const -> DistributionChart*
{
    return static_cast<DistributionChart*>(m_distributionChartView->chart());
}