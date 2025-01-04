#pragma once

#include <QtCharts/qchart.h>

class SDEChart : public QChart{

    Q_OBJECT

public:
    explicit SDEChart();
    auto OnButtonClickSignal(
        const std::vector<double>& sampleData,
        const QString& title) -> void;

private:
    auto PlotChart(const std::vector<double>& sampleData) -> void;
    auto UpdateRangesIfNeeded(double maxX, double minY, double maxY) -> void;
    auto InitializeSDEChart() -> void;
};