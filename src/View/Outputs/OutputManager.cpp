#include "PathChart.hpp"
#include "OutputManager.hpp"


OutputManager::OutputManager(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this)),
    m_chart(new PathChart()),
    m_chartView(new QChartView(m_chart, this))
{   
    m_layout->addWidget(m_chartView);
    setLayout(m_layout);
}

auto OutputManager::OnPathReceived(const PathQuery& query, const Path& sampleData) const -> void
{
    m_chart->UpdatePathChart(sampleData, query);
    // Send last datapoint to distributionchart
}