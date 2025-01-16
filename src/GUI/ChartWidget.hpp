#pragma once

#include "PathChart.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qchart.h>

class ChartWidget : public QWidget{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget* parent = nullptr);

public slots:
    auto SDEButtonClicked(const std::vector<double>& sampleData, const QString& title) -> void;

private:
    QVBoxLayout* m_layout;
    PathChart* m_chart;
    QChartView* m_chartView;
};