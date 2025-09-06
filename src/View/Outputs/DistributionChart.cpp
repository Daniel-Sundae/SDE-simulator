#include "DistributionChart.hpp"
#include "ViewUtils.hpp"
#include "PathQuery.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarCategoryAxis>

inline static constexpr size_t s_nrBins = 100;

DistributionChart::DistributionChart()
    : QChart()
    , m_xAxis(new QValueAxis(this))
    , m_categoryAxis(new QBarCategoryAxis(this))
    , m_yAxisRelativeCount(new QValueAxis(this))
    , m_yAxisDensity(new QValueAxis(this))
    , m_expValLine(new QLineSeries(this))
    , m_pdf(new QLineSeries(this))
    , m_distributionSet(new QBarSet("Distribution", this))
    , m_distributionSeries(new QBarSeries(this))
{
    initializeAxis();
    initializeDistributionChart();
}

void DistributionChart::updateTitle(const ProcessType type){
    QString title;
    QTextStream ts(&title);
    ts << "Sampled distribution";
    const std::string_view name = getField(FieldTags::name{}, type);
    if (!name.empty()){
        ts << " vs " << QString::fromUtf8(name.data(), int(name.size())) << " theoretical Probability Density Function";
    }
    setTitle(title);
}

void DistributionChart::plotDistribution(const Distribution& results){
    Utils::assertTrue(m_distributionSet->count() == 0, "Distribution set is not empty");
    if (results.empty()) return;
    const State xMin = m_xAxis->min();
    const State xMax = m_xAxis->max();
    const State range = xMax - xMin;
    Utils::assertTrue(range > 0, "x-axis range must be positive");
    const State binWidth = range / static_cast<qreal>(s_nrBins);
    Utils::assertTrue(binWidth > 0, "Bin width must be positive");
    std::vector<size_t> histogram(s_nrBins, 0);
    for (double res : results) {
        Utils::assertTrue(xMin <= res && res <= xMax, "Result is out of bounds");
        size_t binIndex = static_cast<size_t>((res - xMin) / binWidth);
        if(binIndex == s_nrBins) binIndex--; // Put rightmost point in last bin
        histogram[binIndex]++;
    }
    // Relative count is the percent of the total path endpoints that fall into each bin
    State maxBinRelativeCountInPercent = 0.0;
    for (size_t bin = 0; bin < s_nrBins; ++bin) {
        size_t count = histogram[bin];
        double binRelativeCountInPercent = static_cast<double>(count) / results.size() * 100.0;
        m_distributionSet->append(binRelativeCountInPercent);
        maxBinRelativeCountInPercent = std::max(maxBinRelativeCountInPercent, binRelativeCountInPercent);
    }
    m_yAxisRelativeCount->setRange(0, maxBinRelativeCountInPercent);
}

void DistributionChart::clearDistributionChart(){
    m_pdf->clear();
    m_expValLine->clear();
    m_distributionSet->remove(0, m_distributionSet->count());
    m_yAxisDensity->setRange(0, 100);
}

void DistributionChart::updateDistributionChartPDF(const PDFData& pdfData){
    if (pdfData.empty()) return;
    QVector<QPointF> points;
    points.reserve(static_cast<qsizetype>(pdfData.size()));
    Density maxDensity = 0.0;
    const State xMin = m_xAxis->min();
    const State xMax = m_xAxis->max();
    const State range = xMax - xMin;
    Utils::assertTrue(range > 0, "x-axis range must be positive");
    const double increment = range/static_cast<qreal>(pdfData.size());
    for (size_t i = 0; i < pdfData.size(); ++i) {
        points.append(QPointF(xMin + static_cast<double>(i)*increment, pdfData[i]));
        maxDensity = std::max(maxDensity, pdfData[i]);
    }
    m_pdf->replace(points);
    m_yAxisDensity->setRange(0, maxDensity);
}

void DistributionChart::plotExpValLine(const State EV){
    const auto y0 = m_yAxisDensity->min();
    const auto y1 = m_yAxisDensity->max();
    m_expValLine->replace({ QPointF(EV, y0), QPointF(EV, y1) });
    m_xAxis->setTickAnchor(EV);
}

void DistributionChart::setDistributionChartSupport(const Range range){
    m_xAxis->setRange(range.first, range.second);
}

void DistributionChart::initializeAxis(){
    addAxis(m_xAxis, Qt::AlignBottom);
    addAxis(m_yAxisRelativeCount, Qt::AlignLeft);
    addAxis(m_yAxisDensity, Qt::AlignRight);
    addAxis(m_categoryAxis, Qt::AlignBottom);
    m_xAxis->setTitleText("X<sub>T</sub>");
    m_xAxis->setTickCount(7);
    m_xAxis->setGridLineVisible(false);
    m_xAxis->setRange(-10, 10);
    m_yAxisRelativeCount->setTitleText("Count (%)");
    m_yAxisRelativeCount->setGridLineVisible(false);
    m_yAxisRelativeCount->setRange(0, 1);
    m_yAxisDensity->setVisible(false);
    m_yAxisDensity->setGridLineVisible(false);
    m_yAxisDensity->setRange(0, 100);

    for (size_t i = 0; i < s_nrBins; ++i) {
        m_categoryAxis->append(QString::number(i));
    }
    m_categoryAxis->setVisible(false);
}

void DistributionChart::initializeDistributionChart(){
    GUI::setChartStyle(this);
    addSeries(m_pdf);
    addSeries(m_expValLine);
    addSeries(m_distributionSeries);
    m_pdf->attachAxis(m_xAxis);
    m_pdf->attachAxis(m_yAxisDensity);
    m_pdf->setName("Probability Density Function");
    m_expValLine->setPen(QPen(Qt::green, 2.0, Qt::DashLine));
    m_expValLine->attachAxis(m_xAxis);
    m_expValLine->attachAxis(m_yAxisDensity);
    m_expValLine->setName("Expected Value");
    m_distributionSeries->attachAxis(m_categoryAxis);
    m_distributionSeries->attachAxis(m_yAxisRelativeCount);
    m_distributionSeries->append(m_distributionSet);
    m_distributionSeries->setBarWidth(1);
    GUI::setPDFStyle(m_pdf);
}
