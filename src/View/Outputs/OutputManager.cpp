#include "PathChart.hpp"
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
    m_layout->addWidget(m_distributionChartView);
    setLayout(m_layout);
}

auto OutputManager::GetPathChart() const -> PathChart*
{
    return static_cast<PathChart*>(m_pathChartView->chart());
}

auto OutputManager::UpdateChartTitle(const PathQuery& pQuery) const -> void
{
    GetPathChart()->UpdateTitle(pQuery);
}

auto OutputManager::PlotDriftLine(const Path& driftLine) const -> void
{
    GetPathChart()->PlotDriftLine(driftLine);
}

auto OutputManager::PlotPath(const Path& path) const -> void
{
    GetPathChart()->PlotPath(path);
}

auto OutputManager::Clear() -> void
{
    GetPathChart()->ClearPaths();

    // Send last datapoint to distributionchart
}
