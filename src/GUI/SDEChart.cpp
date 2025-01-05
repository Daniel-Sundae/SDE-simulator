#include "SDEChart.hpp"
#include "QtCharts/qlineseries.h"
#include "QtCharts/qvalueaxis.h"

SDEChart::SDEChart() : QChart()
{
    InitializeSDEChart();
}

auto SDEChart::OnButtonClickSignal(const std::vector<double>& sampleData, const QString& title) -> void
{
    setTitle(title);
    PlotChart(sampleData);
}

auto SDEChart::PlotChart(const std::vector<double>& sampleData) -> void
{
    if (sampleData.empty()) return;
    removeAllSeries();
    QLineSeries* series = new QLineSeries();
    for(size_t i = 0; i < sampleData.size(); ++i) {
        series->append(static_cast<qreal>(i), static_cast<qreal>(sampleData[i]));
    }
    addSeries(series);

    auto xAxis = qobject_cast<QValueAxis*>(axes(Qt::Horizontal).first());
    auto yAxis = qobject_cast<QValueAxis*>(axes(Qt::Vertical).first());
    if (xAxis && yAxis) {
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
    }
    auto [min_it, max_it] = std::minmax_element(sampleData.begin(), sampleData.end());
    UpdateRangesIfNeeded(sampleData.size(), *min_it, *max_it);
}

auto SDEChart::UpdateRangesIfNeeded(double sampleMaxX, double sampleMinY, double sampleMaxY) -> void
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

auto SDEChart::InitializeSDEChart() -> void
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

