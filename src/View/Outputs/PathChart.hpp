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
    void plotPaths(Paths&& _paths);
    void preparePlotPath(Path&& path, int maxPoints, double xMin, double xMax);
    void plotPath(QVector<QPointF> points);
    void clearPathChart();
    void plotDriftCurve(const Path& driftLine);
    void setMaxTime(const Time time);
    void setYAxisRange(const State min, const State max);
private:
    void initializeAxis();
    void initializePathChart();
private:
    QValueAxis* m_xAxisTime;
    QValueAxis* m_yAxis;
    QLineSeries* m_zeroLine;
    QLineSeries* m_driftCurve;
};
