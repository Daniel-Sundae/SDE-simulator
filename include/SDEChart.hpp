#pragma once

#include <QtCharts/qchart.h>

class SDEChart : public QChart{

    Q_OBJECT

public:
    explicit SDEChart();
    auto PlotChart(const std::vector<double>& sampleData) -> void;

private:
    auto UpdateRangesIfNeeded(double maxX, double minY, double maxY) -> void;
    auto InitializeSDEChart() -> void;
};