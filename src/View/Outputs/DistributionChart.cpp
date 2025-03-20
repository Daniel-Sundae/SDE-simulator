#include "DistributionChart.hpp"
#include "ViewUtils.hpp"
#include <algorithm>
#include <QtCharts/qbarset.h>
#include <QtCharts/qbarseries.h>
#include <QtCharts/qvalueaxis.h>
#include <QtCharts/qlineseries.h>

DistributionChart::DistributionChart()
	: QChart()
	, m_xAxis(new QValueAxis(this))
	, m_yAxisRelativeCount(new QValueAxis(this))
	, m_yAxisDensity(new QValueAxis(this))
	, m_expValLine(nullptr)
	, m_pdf(new QLineSeries(this))
	, m_distribution(new QBarSeries(this))
{
	InitializeAxis();
	InitializeDistributionChart();
}

auto DistributionChart::UpdateTitle(const ProcessType type) -> void
{
	QString title;
	QTextStream ts(&title);
	ts << "Sampled distribution";
	if (ProcessData::GetPDF(type, 0, 0, 0, 0)){
		ts << " vs theoretical Probability Density Function";
	}
	setTitle(title);
}

auto DistributionChart::PlotDistribution(const Distribution& results) -> void
{
	assert(m_distribution->count() == 1);
	const std::size_t numBins = 2;
	const double binWidth = (m_xAxis->max() - m_xAxis->min()) / static_cast<qreal>(numBins);
	std::vector<std::size_t> histogram(numBins, 0);
	for (double res : results) {
		std::size_t binIndex = static_cast<std::size_t>((res - m_xAxis->min()) / static_cast<qreal>(binWidth));
		histogram[binIndex]++;
	}
	double maxCount = 0.0;
	for (std::size_t count : histogram) {
		double relCount = static_cast<double>(count) / static_cast<double>(results.size());
		*DistributionSet() << relCount;
		maxCount = std::max(maxCount, relCount);
	}
	m_yAxisRelativeCount->setRange(0, maxCount * 1.1);
}

auto DistributionChart::DistributionSet() const -> QBarSet*
{
	return m_distribution->barSets().at(0);
}

auto DistributionChart::ClearDistributionChart() -> void
{
	DistributionSet()->remove(0, DistributionSet()->count());
	m_pdf->clear();
}

auto DistributionChart::UpdateDistributionChartPDF(const PDFData& pdfData) -> void
{
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

auto DistributionChart::PlotExpValLine(const State EV) -> void
{
	(void)EV;
    // GUI::SetDriftStyle(series);

}

auto DistributionChart::SetDistributionChartSupport(const Range range) -> void
{
    m_xAxis->setRange(range.first, range.second);
}

auto DistributionChart::InitializeAxis() -> void
{
	addAxis(m_xAxis, Qt::AlignBottom);
	addAxis(m_yAxisRelativeCount, Qt::AlignLeft);
	addAxis(m_yAxisDensity, Qt::AlignRight);
	m_yAxisDensity->setVisible(false);
	m_xAxis->setTitleText("X<sub>T</sub>");
	m_yAxisRelativeCount->setTitleText("Count (%)");
	m_xAxis->setGridLineVisible(false);
	m_yAxisRelativeCount->setGridLineVisible(false);
	m_yAxisDensity->setGridLineVisible(false);
	m_xAxis->setRange(-10, 10);
	m_yAxisRelativeCount->setRange(0, 1);
	m_yAxisDensity->setRange(0, 1);
}

auto DistributionChart::InitializeDistributionChart() -> void
{
	GUI::SetChartStyle(this);
	addSeries(m_pdf);
	m_pdf->attachAxis(m_xAxis);
	m_pdf->attachAxis(m_yAxisDensity);
	m_pdf->setName("Probability Density Function");
	addSeries(m_distribution);
	m_distribution->attachAxis(m_xAxis);
	m_distribution->attachAxis(m_yAxisRelativeCount);
	QBarSet* set = new QBarSet("Distribution");
	m_distribution->append(set);
	GUI::SetPDFStyle(m_pdf);
}

