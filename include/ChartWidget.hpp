#pragma once

#include "SDEChart.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qchart.h>

class ChartWidget : public QWidget{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget* parent = nullptr);
    auto PlotChart(const std::vector<double>& sampleData) -> void;

private:
    QVBoxLayout* m_layout;
    SDEChart* m_chart;
    QChartView* m_chartView;
};