#pragma once

#include "PathChart.hpp"
#include "Types.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qchart.h>


class OutputManager : public QWidget{
    Q_OBJECT

public:
    explicit OutputManager(QWidget* parent = nullptr);

    auto OnPathReceived(const Path& sampleData) const -> void;

private:
    QVBoxLayout* m_layout;
    PathChart* m_chart;
    QChartView* m_chartView;
};