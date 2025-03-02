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

    // PathChart
    auto UpdateChartTitle(const PathQuery& pQuery) const -> void;
    auto PlotPath(const Path& path) const -> void;
    auto PlotDriftLine(const Path& driftLine) const -> void;
    auto Clear() -> void;

    // DistributionChart


private:
    auto GetPathChart() const->PathChart*;
    //auto GetDirstributionChart() const->DistributionChart*;
private:
    QVBoxLayout* m_layout;
    QChartView* m_pathChartView;
    QChartView* m_distributionChartView;
};