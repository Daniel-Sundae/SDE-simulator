#pragma once
#include "Types.hpp"
#include <QtCharts/qchart.h>

class PathChart : public QChart{
    Q_OBJECT

public:
    explicit PathChart();
    auto UpdatePathChart(const Path &sampleData, const ProcessType type) -> void;

private:
    auto PlotChart(const Path& sampleData) -> void;
    auto UpdateRangesIfNeeded(double maxX, double minY, double maxY) -> void;
    auto InitializeProcessChart() -> void;
};