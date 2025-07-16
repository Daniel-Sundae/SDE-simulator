#pragma once
#include "PathQuery.hpp"
#include <QtCharts/QChart>

class QChartView;
class QValueAxis;
class QLineSeries;

class PathChart : public QChart{
    Q_OBJECT

public:
    explicit PathChart();
    void updateTitle(bool allPathsPlotted);
    void plotPath(const Path& sampleData);
    void clearPathChart(bool clearDrift = true);
    void plotDriftCurve(const Path& driftLine);
    void setMaxTime(const Time time);
private:
    void updateZeroLine();
    void updateYAxisIfNeeded(State min_Y, State max_Y);
    void initializeAxis();
    void initializeProcessChart();
private:
    QValueAxis* m_xAxisTime;
    QValueAxis* m_yAxis;
    QLineSeries* m_zeroLine;
    QLineSeries* m_driftCurve;
};
