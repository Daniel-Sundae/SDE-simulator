#include "DistributionChart.hpp"
#include "ViewUtils.hpp"
#include "PathQuery.hpp"
#include <algorithm>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarCategoryAxis>

static size_t nrBins = 100;

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
	if (ProcessData::getPDF(type, 0, 0, 0, 0)){
		ts << " vs theoretical Probability Density Function";
	}
	setTitle(title);
}

void DistributionChart::plotDistribution(const Distribution& results){
	assert(m_distributionSet->count() == 0);
	if(results.empty()) return;
	const State binWidth = (m_xAxis->max() - m_xAxis->min()) / static_cast<qreal>(nrBins);
	std::vector<size_t> histogram(nrBins, 0);
	for (double res : results) {
		size_t binIndex = static_cast<size_t>((res - m_xAxis->min()) / static_cast<qreal>(binWidth));
		histogram[binIndex]++;
	}
	double maxHeight = 0.0;
	for (size_t bin = 0; bin < histogram.size(); ++bin) {
		size_t count = histogram[bin];
		double binHeight = static_cast<double>(count) / results.size();
		m_distributionSet->append(binHeight);
		maxHeight = std::max(maxHeight, binHeight);
	}
	m_yAxisRelativeCount->setRange(0, maxHeight * 1.1);
}

void DistributionChart::clearDistributionChart(){
	m_pdf->clear();
	m_expValLine->clear();
	m_distributionSet->remove(0, m_distributionSet->count());
}

void DistributionChart::updateDistributionChartPDF(const PDFData& pdfData){
    if (pdfData.empty()) return;
    QVector<QPointF> points;
    points.reserve(static_cast<qsizetype>(pdfData.size()));

    Density maxDensity = 0.0;
	const double increment = (m_xAxis->max()-m_xAxis->min())/static_cast<qreal>(pdfData.size());
    for (size_t i = 0; i < pdfData.size(); ++i) {
        points.append(QPointF(m_xAxis->min() + static_cast<double>(i)*increment, pdfData[i]));
		maxDensity = std::max(maxDensity, pdfData[i]);
    }
	m_pdf->replace(points);
	m_yAxisDensity->setRange(0, maxDensity * 1.1);
}

void DistributionChart::plotExpValLine(const State EV){
	QVector<QPointF> points;
    points.reserve(2);
    points.append(QPointF(EV, 0));
    points.append(QPointF(EV, m_yAxisDensity->max()*1.1));
	m_expValLine->replace(points);
}

void DistributionChart::setDistributionChartSupport(const Range range){
    m_xAxis->setRange(range.first, range.second);
}

void DistributionChart::initializeAxis(){
	addAxis(m_xAxis, Qt::AlignBottom);
	addAxis(m_yAxisRelativeCount, Qt::AlignLeft);
	addAxis(m_yAxisDensity, Qt::AlignRight);
	addAxis(m_categoryAxis, Qt::AlignBottom);
	m_yAxisDensity->setVisible(false);
	m_xAxis->setTitleText("X<sub>T</sub>");
	m_yAxisRelativeCount->setTitleText("Count (%)");
	m_xAxis->setGridLineVisible(false);
	m_yAxisRelativeCount->setGridLineVisible(false);
	m_yAxisDensity->setGridLineVisible(false);
	m_xAxis->setRange(-10, 10);
	m_yAxisRelativeCount->setRange(0, 1);
	m_yAxisDensity->setRange(0, 1);
	for (size_t i = 0; i < nrBins; ++i) {
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
	m_expValLine->setPen(QPen(Qt::gray, 3.0, Qt::DashLine));
	m_expValLine->attachAxis(m_xAxis);
	m_expValLine->attachAxis(m_yAxisDensity);
	m_expValLine->setName("Expected Value");
	m_distributionSeries->attachAxis(m_categoryAxis);
	m_distributionSeries->attachAxis(m_yAxisRelativeCount);
	m_distributionSeries->append(m_distributionSet);
	m_distributionSeries->setBarWidth(1);
	GUI::setPDFStyle(m_pdf);
}
