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
	, m_theoreticalPDF(nullptr)
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
	const std::size_t flooredSize = static_cast<std::size_t>(std::floor(results.size()/2));
	const std::size_t numBins = (flooredSize > 20) ? flooredSize : 20;
	const double binWidth = (m_xAxis->max() - m_xAxis->min()) / numBins;

	std::vector<std::size_t> histogram(numBins, 0);
	for (double res : results) {
		std::size_t binIndex = static_cast<std::size_t>((res - m_xAxis->min()) / binWidth);
		histogram[binIndex]++;
	}
	QBarSeries* series = new QBarSeries(this);
	QBarSet* set = new QBarSet("Distribution");
	for (std::size_t count : histogram) {
		set->append(static_cast<double>(count) / results.size());
	}
	series->append(set);
	addSeries(series);
}

auto DistributionChart::ClearDistributionChart() -> void
{
	removeAllSeries();
}

auto DistributionChart::PlotPDF(const PDFData& pdfData) -> void
{
    if (pdfData.empty()) return;

    QLineSeries* series = new QLineSeries(this);
    QVector<QPointF> points;
    points.reserve(static_cast<qsizetype>(pdfData.size()));

    double yMax = 0.0;
	const double increment = (m_xAxis->max()-m_xAxis->min())/pdfData.size();
    for (size_t i = 0; i < pdfData.size(); ++i) {
        points.append(QPointF(m_xAxis->min() + i*increment, pdfData[i]));
        if (pdfData[i] > yMax)
            yMax = pdfData[i];
    }
    series->replace(points);
    addSeries(series);
    series->attachAxis(m_xAxis);
    series->attachAxis(m_yAxisDensity);
	series->setName("Probability Density Function");
	GUI::SetPDFStyle(series);
	SetMaxYAxisDensity(yMax*1.1);
}

auto DistributionChart::PlotExpValLine(const State EV) -> void
{
	(void)EV;
    // GUI::SetDriftStyle(series);

}

auto DistributionChart::SetXAxisRange(const Range range) -> void
{
    m_xAxis->setRange(range.first, range.second);
}

auto DistributionChart::SetMaxYAxisDensity(const Density yMax) -> void
{
    m_yAxisDensity->setRange(0, yMax);
}

auto DistributionChart::InitializeAxis() -> void
{
	addAxis(m_xAxis, Qt::AlignBottom);
	addAxis(m_yAxisRelativeCount, Qt::AlignLeft);
	addAxis(m_yAxisDensity, Qt::AlignRight);
	m_xAxis->setTitleText("X<sub>T</sub>");
	m_yAxisRelativeCount->setTitleText("Relative count (%)");
	m_yAxisDensity->setTitleText("Pdf (%)");
	m_xAxis->setGridLineVisible(false);
	m_yAxisRelativeCount->setGridLineVisible(false);
	m_yAxisDensity->setGridLineVisible(false);
	m_xAxis->setRange(0, 10);
	m_yAxisRelativeCount->setRange(0, 1);
	m_yAxisDensity->setRange(0, 1);
}

auto DistributionChart::InitializeDistributionChart() -> void
{
	GUI::SetChartStyle(this);
}

