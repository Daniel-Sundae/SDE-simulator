#pragma once
#include "BasicTypes.hpp"
#include "PathQuery.hpp"
#include <QtCharts/qchart.h>

class QChartView;
class QValueAxis;
class QLineSeries;

class PathChart : public QChart{
    Q_OBJECT

public:
    explicit PathChart();
    auto UpdateTitle(const PathQuery& pQuery) -> void;
    auto PlotPath(const Path& sampleData) -> void;
    auto ClearPaths() -> void;
    auto PlotDriftLine(const Path& driftLine) -> void;
private:
    auto UpdateRangesIfNeeded(std::size_t max_X, State min_Y, State max_Y) -> void;
    auto InitializeAxis() -> void;
    auto InitializeProcessChart() -> void;
private:
    QValueAxis* m_xAxis;
    QValueAxis* m_yAxis;
    QLineSeries* m_zeroLine;
};