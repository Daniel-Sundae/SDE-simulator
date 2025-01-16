#pragma once

#include <QtCharts/qchart.h>

class PathChart : public QChart{

    Q_OBJECT

public:
    explicit PathChart();
    auto UpdatePathChart(
        const std::vector<double>& sampleData,
        const QString& title) -> void;

private:
    auto PlotChart(const std::vector<double>& sampleData) -> void;
    auto UpdateRangesIfNeeded(double maxX, double minY, double maxY) -> void;
    auto InitializeSDEChart() -> void;
};