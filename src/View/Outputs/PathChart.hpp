#pragma once
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
    auto ClearPathChart() -> void;
    auto PlotDriftCurve(const Path& driftLine) -> void;
    auto SetMaxTime(const Time time) -> void;
private:
    auto UpdateZeroLine() -> void;
    auto UpdateYAxisIfNeeded(State min_Y, State max_Y) -> void;
    auto InitializeAxis() -> void;
    auto InitializeProcessChart() -> void;
private:
    QValueAxis* m_xAxisTime;
    QValueAxis* m_yAxis;
    QLineSeries* m_zeroLine;
    QLineSeries* m_driftCurve;
};