#pragma once
#include "PathQuery.hpp"
#include <array>
#include <QtCharts/QChart>

class QValueAxis;
class QBarCategoryAxis;
class QLineSeries;
class QBarSeries;
class QBarSet;

class DistributionChart : public QChart {
    Q_OBJECT

public:
    explicit DistributionChart();
    void updateTitle(const ProcessType type);
    void plotDistribution(const Distribution& results);
    void clearDistributionChart();
    void updateDistributionChartPDF(const PDFData& pdfData);
    void plotExpValLine(const State EV);
    void setDistributionChartSupport(const Range range);
private:
    void initializeAxis();
    void initializeDistributionChart();
private:
    QValueAxis* m_xAxis;
    QBarCategoryAxis* m_categoryAxis;
    QValueAxis* m_yAxisRelativeCount;
    QValueAxis* m_yAxisDensity;
    QLineSeries* m_expValLine;
    QLineSeries* m_pdf;
    QBarSet* m_distributionSet;
    QBarSeries* m_distributionSeries;
};
