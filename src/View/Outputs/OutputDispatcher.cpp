#include "PathChart.hpp"
#include "DistributionChart.hpp"
#include "OutputDispatcher.hpp"

OutputDispatcher::OutputDispatcher(QWidget *parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_pathChartView(new QChartView(this))
    , m_distributionChartView(new QChartView(this))
{
    auto* pathChart = new PathChart();
    m_pathChartView->setChart(pathChart);
    m_layout->addWidget(m_pathChartView);
    auto* distributionChart = new DistributionChart();
    m_distributionChartView->setChart(distributionChart);
    m_layout->addWidget(m_distributionChartView);
    setLayout(m_layout);
}


////////////////////////////////////////
//// PATH CHART
////////////////////////////////////////


auto OutputDispatcher::UpdatePathChartTitle(const PathQuery& pQuery) const -> void
{
    GetPathChart()->UpdateTitle(pQuery);
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