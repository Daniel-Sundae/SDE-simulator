#include "PathChart.hpp"
#include "ProcessData.hpp"
#include "Types.hpp"
#include <QtCharts/qlineseries.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qvalueaxis.h>

PathChart::PathChart()
    : QChart()
{
    InitializeProcessChart();
}

auto PathChart::UpdatePathChart(const Path& path, const PathQuery& pQuery) -> void
{
    QString title;
    QTextStream(&title) << QString::fromUtf8(ProcessData::Name(pQuery.processDefinition.type))
        << ": "
        << QString::fromUtf8(ProcessData::Definition(pQuery.processDefinition.type))
        << " with μ = "
        << QString::number(pQuery.processDefinition.drift.Mu())
        << ", σ = "
        << QString::number(pQuery.processDefinition.diffusion.Sigma())
        << ", X<sub>0</sub> = "
        << QString::number(pQuery.processDefinition.startValue);
    setTitle(title);
    PlotChart(path);
}

auto PathChart::PlotChart(const Path& path) -> void
{
    if (path.empty()) return;
    removeAllSeries();
    QLineSeries* series = new QLineSeries(this);
    for(size_t i = 0; i < path.size(); ++i) {
        series->append(static_cast<qreal>(i), static_cast<qreal>(path[i]));
    }
    addSeries(series);

    auto xAxis = qobject_cast<QValueAxis*>(axes(Qt::Horizontal).first());
    auto yAxis = qobject_cast<QValueAxis*>(axes(Qt::Vertical).first());
    if (xAxis && yAxis) {
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
    }
    auto [min_it, max_it] = std::minmax_element(path.begin(), path.end());
    UpdateRangesIfNeeded(path.size(), *min_it, *max_it);
}

auto PathChart::UpdateRangesIfNeeded(std::size_t max_X, State min_Y, State max_Y) -> void
{
    auto xAxis = qobject_cast<QValueAxis*>(axes(Qt::Horizontal).first());
    auto yAxis = qobject_cast<QValueAxis*>(axes(Qt::Vertical).first());

    if (!xAxis || !yAxis) return;

    // X-axis handling
    // Use qreal (double) for Qt axis operations
    qreal currentXMax = xAxis->max();
    // Convert size_t to qreal for comparison
    currentXMax = std::max(currentXMax, static_cast<qreal>(max_X));
    if (currentXMax > 3 * static_cast<qreal>(max_X)) {
        currentXMax = static_cast<qreal>(max_X);
    }
    xAxis->setRange(0, currentXMax);

    // Y-axis handling
    qreal currentYMin = yAxis->min();
    qreal currentYMax = yAxis->max();

    // Convert State to qreal for calculations
    qreal paddedMinY = static_cast<qreal>(min_Y) * 1.1;
    qreal paddedMaxY = static_cast<qreal>(max_Y) * 1.1;

    qreal hysteresis = (currentYMax - currentYMin) * 0.05;

    bool needsYAdjustment = false;
    qreal newMin = currentYMin;
    qreal newMax = currentYMax;

    if (paddedMinY < currentYMin - hysteresis ||
        paddedMaxY > currentYMax + hysteresis) {

        qreal extraPadding = (paddedMaxY - paddedMinY) * 0.1;
        newMin = std::min(currentYMin, paddedMinY - extraPadding);
        newMax = std::max(currentYMax, paddedMaxY + extraPadding);
        needsYAdjustment = true;
    }

    qreal currentRange = currentYMax - currentYMin;
    qreal dataRange = static_cast<qreal>(max_Y - min_Y);
    if (currentRange > 8 * dataRange) {
        newMin = paddedMinY;
        newMax = paddedMaxY;
        needsYAdjustment = true;
    }

    if (needsYAdjustment) {
        newMin = std::floor(newMin / 10.0) * 10;
        newMax = std::ceil(newMax / 10.0) * 10;

        if (newMin > 0) newMin = 0;
        if (newMax < 0) newMax = 0;

        yAxis->setRange(newMin, newMax);
        yAxis->setTickCount(static_cast<int>((newMax - newMin) / 10 + 1));
    }
}

auto PathChart::InitializeProcessChart() -> void
{
    setTheme(QChart::ChartThemeBlueNcs);
    legend()->setVisible(false);
    QValueAxis* xAxis = new QValueAxis(this);
    QValueAxis* yAxis = new QValueAxis(this);
    addAxis(xAxis, Qt::AlignBottom);
    addAxis(yAxis, Qt::AlignLeft);
    xAxis->setTitleText("t (s)");
    yAxis->setTitleText("X<sub>t</sub>");
    xAxis->setGridLineVisible(true);
    yAxis->setGridLineVisible(true);
    xAxis->setRange(0, 10);  // Set some default range
    yAxis->setRange(-10, 10);  // Set some default range
    yAxis->setTickCount(3);
    yAxis->setMinorTickCount(2);
}

