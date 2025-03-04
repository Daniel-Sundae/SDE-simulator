#include "PathChart.hpp"
#include "DistributionChart.hpp"
#include "OutputManager.hpp"

OutputManager::OutputManager(QWidget *parent)
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


auto OutputManager::UpdatePathChartTitle(const PathQuery& pQuery) const -> void
{
    GetPathChart()->UpdateTitle(pQuery);
}

auto OutputManager::PlotPathChartDriftLine(const Path& driftLine) const -> void
{
    GetPathChart()->PlotDriftLine(driftLine);
}

auto OutputManager::PlotPath(const Path& path) const -> void
{
    GetPathChart()->PlotPath(path);
}

auto OutputManager::ClearPaths() -> void
{
    GetPathChart()->ClearPaths();
}

////////////////////////////////////////
//// DISTRIBUTION CHART
////////////////////////////////////////

auto OutputManager::UpdateDistributionChartTitle(const PathQuery& pQuery) const -> void
{
    GetDistributionChart()->UpdateTitle(pQuery);
}

auto OutputManager::PlotDistribution(const std::vector<State>& results) const -> void
{
    GetDistributionChart()->PlotDistribution(results);
}

auto OutputManager::ClearDistribution() -> void
{
    GetDistributionChart()->ClearDistribution();
}

auto OutputManager::GetPathChart() const -> PathChart*
{
    return static_cast<PathChart*>(m_pathChartView->chart());
}

auto OutputManager::GetDistributionChart() const -> DistributionChart*
{
    return static_cast<DistributionChart*>(m_distributionChartView->chart());
}