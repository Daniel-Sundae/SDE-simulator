#include "ChartWidget.hpp"

ChartWidget::ChartWidget(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this)),
    m_chart(new QChart()),
    m_chartView(new QChartView(m_chart, this))
{   
    m_layout->addWidget(m_chartView);
    setLayout(m_layout);
}
