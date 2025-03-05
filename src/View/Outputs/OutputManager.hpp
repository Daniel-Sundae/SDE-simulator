#pragma once

#include "Types.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qchart.h>

class PathQuery;
class PathChart;
class DistributionChart;

class OutputManager : public QWidget{
    Q_OBJECT

public:
    explicit OutputManager(QWidget* parent = nullptr);

    // PathChart
    auto UpdatePathChartTitle(const PathQuery& pQuery) const -> void;
    auto PlotPath(const Path& path) const -> void;
    auto PlotPathChartDriftLine(const Path& driftLine) const -> void;
    auto ClearPaths() -> void;

    // DistributionChart
    auto UpdateDistributionChartTitle(const PathQuery& pQuery) const -> void;
    auto PlotDistribution(const std::vector<State>& results) const -> void;
    auto ClearDistribution() -> void;

private:
    auto GetPathChart() const -> PathChart*;
    auto GetDistributionChart() const -> DistributionChart*;
private:
    QVBoxLayout* m_layout;
    QChartView* m_pathChartView;
    QChartView* m_distributionChartView;
};