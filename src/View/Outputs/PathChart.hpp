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
    void plotPaths(Paths&& paths);
    void plotPath(QVector<QPointF> points);
    void clearPathChart();
    void plotDriftCurve(const Path& drift, State minXt, State maxXt);
    void setMaxTime(const Time time);
    void expandYAxisRange(const State min, const State max);
private:
    void initializeAxis();
    void initializePathChart();
private:
    QValueAxis* m_xAxisTime;
    QValueAxis* m_yAxis;
    QLineSeries* m_zeroLine;
    QLineSeries* m_driftCurve;
};
