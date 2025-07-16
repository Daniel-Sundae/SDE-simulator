#pragma once

#include "Types.hpp"
#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>

struct PathQuery;
class PathChart;
class DistributionChart;
class StatusManager; // Forward declaration

class OutputDispatcher : public QWidget{
    Q_OBJECT

public:
    explicit OutputDispatcher(QWidget* parent = nullptr);

    // StatusManager
    void setQueryInfo(const PathQuery &pQuery) const;
    void setStatus(const StatusSignal signal) const;
    void clearStatus() const;

    // PathChart
    void plotPath(const Path& path) const;
    void updatePathChartTitle(bool allPathsPlotted) const;
    void plotPathChartDriftData(const Path &driftLine) const;
    void clearPathChart(bool clearDrift = true);
    void setPathChartMaxTime(const Time time);

    // DistributionChart
    void updateDistributionChartTitle(const ProcessType t) const;
    void plotDistribution(const Distribution& distribution) const;
    void updateDistributionChartPDF(const PDFData& pdfData) const;
    void setDistributionChartSupport(const Range pdfDomain) const;
    void updateDistributionChartEV(const State EV) const;
    void clearDistributionChart();

private:
    PathChart* getPathChart() const;
    DistributionChart* getDistributionChart() const;
private:
    QVBoxLayout* layout;
    StatusManager* m_statusManager;
    QChartView* m_pathChartView;
    QChartView* m_distributionChartView;
};
