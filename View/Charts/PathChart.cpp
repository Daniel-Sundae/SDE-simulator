#include "PathChart.hpp"
#include "ProcessData.hpp"
#include "Types.hpp"
#include "QtCharts/qlineseries.h"
#include "QtCharts/qvalueaxis.h"

PathChart::PathChart() : QChart()
{
    InitializeProcessChart();
}

auto PathChart::UpdatePathChart(const Path& path, const ProcessType type) -> void
{
    ProcessMetaData metaData = ProcessData::Create(type);
    QString title = metaData.name + ": " + metaData.definition;
    setTitle(title);
    PlotChart(path);
}

auto PathChart::PlotChart(const Path& path) -> void
{
    if (path.empty()) return;
    removeAllSeries();
    QLineSeries* series = new QLineSeries();
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

auto PathChart::UpdateRangesIfNeeded(double sampleMaxX, double sampleMinY, double sampleMaxY) -> void
{
    auto xAxis = qobject_cast<QValueAxis*>(axes(Qt::Horizontal).first());
    auto yAxis = qobject_cast<QValueAxis*>(axes(Qt::Vertical).first());

    if (!xAxis || !yAxis) return;

    // X-axis handling
    double currentXMax = xAxis->max();
    currentXMax = std::max(currentXMax, sampleMaxX);
    if(currentXMax > 3 * sampleMaxX) {
        currentXMax = sampleMaxX;
    }
    xAxis->setRange(0, currentXMax);

    // Y-axis handling
    double currentYMin = yAxis->min();
    double currentYMax = yAxis->max();
    
    // Add 10% padding to the actual data values
    double paddedMinY = sampleMinY * 1.1;
    double paddedMaxY = sampleMaxY * 1.1;

    // Define hysteresis threshold
    double hysteresis = (currentYMax - currentYMin) * 0.05;
    
    bool needsYAdjustment = false;
    double newMin = currentYMin;
    double newMax = currentYMax;

    // Check if adjustment is needed
    if (paddedMinY < currentYMin - hysteresis || 
        paddedMaxY > currentYMax + hysteresis) {
        
        double extraPadding = (paddedMaxY - paddedMinY) * 0.1;
        newMin = std::min(currentYMin, paddedMinY - extraPadding);
        newMax = std::max(currentYMax, paddedMaxY + extraPadding);
        needsYAdjustment = true;
    }

    // Force reset if range becomes too large
    double currentRange = currentYMax - currentYMin;
    double dataRange = sampleMaxY - sampleMinY;
    if (currentRange > 8 * dataRange) {
        newMin = paddedMinY;
        newMax = paddedMaxY;
        needsYAdjustment = true;
    }

    if (needsYAdjustment) {
        // Round to nearest multiple of 10
        newMin = std::floor(newMin / 10.0) * 10;
        newMax = std::ceil(newMax / 10.0) * 10;

        // Ensure 0 is always in the range
        if (newMin > 0) newMin = 0;
        if (newMax < 0) newMax = 0;

        yAxis->setRange(newMin, newMax);
        yAxis->setTickCount((newMax - newMin) / 10 + 1); // Set tick count to show multiples of 10
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
    xAxis->setTitleText("Time");
    yAxis->setTitleText("Value");
    xAxis->setGridLineVisible(true);
    yAxis->setGridLineVisible(true);
    xAxis->setRange(0, 10);  // Set some default range
    yAxis->setRange(-10, 10);  // Set some default range
    yAxis->setTickCount(3);
    yAxis->setMinorTickCount(2);
}

