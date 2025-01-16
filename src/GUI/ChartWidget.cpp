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

auto ChartWidget::SDEButtonClicked(const std::vector<double>& sampleData, const QString& title) -> void
{
    m_chart->UpdatePathChart(sampleData, title);
}