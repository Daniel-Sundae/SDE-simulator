#include "PathChart.hpp"
#include "ChartManager.hpp"


ChartManager::ChartManager(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this)),
    m_chart(new PathChart()),
    m_chartView(new QChartView(m_chart, this))
{   
    m_layout->addWidget(m_chartView);
    setLayout(m_layout);
}

auto ChartManager::OnPathReceived(const Path& sampleData) const -> void
{
    m_chart->UpdatePathChart(sampleData, ProcessType::BM);
}