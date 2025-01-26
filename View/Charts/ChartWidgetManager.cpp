#include "ChartWidget.hpp"
#include "PathChart.hpp"


ChartWidget::ChartWidget(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this)),
    m_chart(new PathChart()),
    m_chartView(new QChartView(m_chart, this))
{   
    m_layout->addWidget(m_chartView);
    setLayout(m_layout);
}

auto ChartWidget::ProcessButtonClicked(const Path& sampleData, const ProcessType type) -> void
{
    m_chart->UpdatePathChart(sampleData, type);
}