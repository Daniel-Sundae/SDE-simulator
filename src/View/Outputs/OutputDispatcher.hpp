#pragma once

#include "Types.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qchart.h>

struct PathQuery;
class PathChart;
class DistributionChart;

class OutputDispatcher : public QWidget{
    Q_OBJECT

public:
    explicit OutputDispatcher(QWidget* parent = nullptr);

    // PathChart
    auto UpdatePathChartTitle(const PathQuery& pQuery) const -> void;
    auto PlotPath(const Path& path) const -> void;
    auto PlotPathChartDriftData(const Path& driftLine) const -> void;
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
    QVBoxLayout* m_layout;
    QChartView* m_pathChartView;
    QChartView* m_distributionChartView;
};