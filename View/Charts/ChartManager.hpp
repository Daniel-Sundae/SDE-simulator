#pragma once

#include "PathChart.hpp"
#include "Types.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qchart.h>


class ChartManager : public QWidget{
    Q_OBJECT

public:
    explicit ChartManager(QWidget* parent = nullptr);

    auto OnPathReceived(const Path& sampleData) const -> void;

private:
    QVBoxLayout* m_layout;
    PathChart* m_chart;
    QChartView* m_chartView;
};