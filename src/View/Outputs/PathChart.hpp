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
    void clearPathChart();
    void plotDriftCurve(const Path& driftLine);
    void setMaxTime(const Time time);
private:
    void initializeAxis();
    void initializePathChart();
private:
    QValueAxis* m_xAxisTime;
    QValueAxis* m_yAxis;
    QLineSeries* m_zeroLine;
    QLineSeries* m_driftCurve;
};
