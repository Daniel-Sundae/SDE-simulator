#include "PathChart.hpp"
#include "OutputManager.hpp"
#include "ViewUtils.hpp"


OutputManager::OutputManager(QWidget *parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_pathChartView(new QChartView(this))
    , m_distributionChartView(new QChartView(this))
{   
    auto* pathChart = new PathChart();
    //pathChart->setTheme(QChart::ChartThemeBrownSand);
    GUI::DarkChartTheme(pathChart);
    m_pathChartView->setChart(pathChart);
    m_layout->addWidget(m_pathChartView);
    m_layout->addWidget(m_distributionChartView);
    setLayout(m_layout);
}

auto OutputManager::GetPathChart() const -> PathChart*
{
    return static_cast<PathChart*>(m_pathChartView->chart());
}

auto OutputManager::OnPathReceived(const PathQuery& query, const Path& sampleData) const -> void
{
    GetPathChart()->UpdatePathChart(sampleData, query);
    // Send last datapoint to distributionchart
}

auto OutputManager::Clear() -> void
{
    GetPathChart()->removeAllSeries();

    // Send last datapoint to distributionchart
}
