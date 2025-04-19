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
    auto SetQueryInfo(const PathQuery &pQuery) const -> void;
    auto SetStatus(const StatusSignal signal) const -> void;

    // PathChart
    auto PlotPath(const Path& path) const -> void;
    auto UpdatePathChartTitle(bool allPathsPlotted) const -> void;
    auto PlotPathChartDriftData(const Path &driftLine) const -> void;
    auto ClearPathChart(bool clearDrift = true) -> void;
    auto SetPathChartMaxTime(const Time time) -> void;

    // DistributionChart
    auto UpdateDistributionChartTitle(const ProcessType t) const -> void;
    auto PlotDistribution(const Distribution& distribution) const -> void;
    auto UpdateDistributionChartPDF(const PDFData& pdfData) const -> void;
    auto SetDistributionChartSupport(const Range pdfDomain) const -> void;
    auto UpdateDistributionChartEV(const State EV) const -> void;
    auto ClearDistributionChart() -> void;

private:
    auto GetPathChart() const -> PathChart*;
    auto GetDistributionChart() const -> DistributionChart*;
private:
    QVBoxLayout* layout;
    StatusManager* m_statusManager;
    QChartView* m_pathChartView;
    QChartView* m_distributionChartView;
};
