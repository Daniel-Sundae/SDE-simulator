#include "ChartWidget.hpp"
#include "SDEChart.hpp"


ChartWidget::ChartWidget(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this)),
    m_chart(new SDEChart()),
    m_chartView(new QChartView(m_chart, this))
{   
    m_layout->addWidget(m_chartView);
    setLayout(m_layout);
}

auto ChartWidget::OnButtonClickSignal(const std::vector<double>& sampleData, const QString& title) -> void
{
    m_chart->OnButtonClickSignal(sampleData, title);
}