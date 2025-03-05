#pragma once
#include "PathQuery.hpp"
#include <QtCharts/qchart.h>

class QValueAxis;
class QLineSeries;

class DistributionChart : public QChart {
    Q_OBJECT

public:
    explicit DistributionChart();
    auto UpdateTitle(const PathQuery& pQuery) -> void;
    auto PlotDistribution(const Distribution& results) -> void;
    auto ClearDistribution() -> void;
    auto PlotTheoreticalPDF(const std::vector<Density>& density) -> void;
    auto PlotExpValLine(const State EV) -> void;
private:
    auto InitializeAxis() -> void;
    auto InitializeDistributionChart() -> void;
private:
    QValueAxis* m_xAxis;
    QValueAxis* m_yAxisRelativeCount;
    QValueAxis* m_yAxisPDF;
    QLineSeries* m_expValLine;
    QLineSeries* m_theoreticalPDF;
};