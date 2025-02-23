#pragma once
#include "Types.hpp"
#include <QtCharts/qchart.h>

class QChartView;

class PathChart : public QChart{
    Q_OBJECT

public:
    explicit PathChart();
    auto UpdatePathChart(const Path& path, const PathQuery& pQuery) -> void;

private:
    auto PlotChart(const Path& sampleData) -> void;
    auto UpdateRangesIfNeeded(std::size_t max_X, State min_Y, State max_Y) -> void;
    auto InitializeProcessChart() -> void;
};