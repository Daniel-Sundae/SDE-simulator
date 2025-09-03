#include "OutputDispatcher.hpp"
#include <QtWidgets/QGraphicsLayout>

OutputDispatcher::OutputDispatcher(QWidget *parent)
    : QWidget(parent)
    , m_statusManager(new StatusManager(this))
    , m_pathChartView(new QChartView(this))
    , m_distributionChartView(new QChartView(this))
{
    layout = new QVBoxLayout(this);
    // Wrapper is only for aligning the Status Manager with the charts
    QVBoxLayout* groupBoxWrapperLayout = new QVBoxLayout();
    groupBoxWrapperLayout->setContentsMargins(8, 0, 8, 0);
    groupBoxWrapperLayout->addWidget(m_statusManager);
    layout->addLayout(groupBoxWrapperLayout);

    auto* pathChart = new PathChart();
    m_pathChartView->setChart(pathChart);
    layout->addWidget(m_pathChartView);

    auto* distributionChart = new DistributionChart();
    m_distributionChartView->setChart(distributionChart);
    layout->addWidget(m_distributionChartView);
}