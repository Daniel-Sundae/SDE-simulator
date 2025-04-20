#include "PathChart.hpp"
#include "ProcessData.hpp"
#include "Types.hpp"
#include "ViewUtils.hpp"
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

PathChart::PathChart()
    : QChart()
    , m_xAxisTime(new QValueAxis(this))
    , m_yAxis(new QValueAxis(this))
    , m_zeroLine(new QLineSeries(this))
    , m_driftCurve(new QLineSeries(this))
{
    InitializeAxis();
    InitializeProcessChart();
}

auto PathChart::UpdateTitle(bool allPathsPlotted) -> void
{
    QString title = allPathsPlotted ? "" : QString("Only plotting first %1 paths").arg(DefaultConstants::maxPathsToDraw);
    setTitle(title);
}

auto PathChart::ClearPathChart(bool clearDrift) -> void
{
    for (QAbstractSeries* s : series()) {
        if (s != m_zeroLine && s != m_driftCurve) {
            removeSeries(s);
            delete s;
        }
    }
    if (clearDrift) {
        m_driftCurve->clear();
    }
}

auto PathChart::PlotDriftCurve(const Path& driftCurve) -> void
{
    QVector<QPointF> points;
    points.reserve(static_cast<qsizetype>(driftCurve.size()));
    const double intervalWidth = (m_xAxisTime->max() - m_xAxisTime->min())/static_cast<qreal>(driftCurve.size());
    State min_y = 0;
    State max_y = 0;
    for (size_t i = 0; i < driftCurve.size(); ++i) {
        min_y = std::min(min_y, driftCurve[i]);
        max_y = std::max(max_y, driftCurve[i]);
        points.append(QPointF(static_cast<double>(i)*intervalWidth, driftCurve[i]));
    }
    m_driftCurve->replace(points);
    UpdateYAxisIfNeeded(min_y, max_y);
}

auto PathChart::PlotPath(const Path& path) -> void
{
    if (path.empty()) return;

    QLineSeries* series = new QLineSeries(this);
    addSeries(series);
    QVector<QPointF> points;
    points.reserve(static_cast<qsizetype>(path.size()));
    const double intervalWidth = (m_xAxisTime->max() - m_xAxisTime->min())/static_cast<qreal>(path.size());
    State min_y = 0;
    State max_y = 0;
    for (size_t i = 0; i < path.size(); ++i) {
        min_y = std::min(min_y, path[i]);
        max_y = std::max(max_y, path[i]);
        points.append(QPointF(static_cast<double>(i)*intervalWidth , path[i]));
    }
    series->replace(points);
    series->attachAxis(m_xAxisTime);
    series->attachAxis(m_yAxis);
    GUI::SetPathStyle(series);
}

auto PathChart::SetMaxTime(const Time time) -> void
{
    m_xAxisTime->setRange(0, time);
    UpdateZeroLine();
}

auto PathChart::UpdateZeroLine() -> void
{
    m_zeroLine->clear();
    m_zeroLine->append(m_xAxisTime->min(), 0);
    m_zeroLine->append(m_xAxisTime->max(), 0);
}

auto PathChart::UpdateYAxisIfNeeded(State min_Y, State max_Y) -> void
{
    constexpr qreal yPaddingFactor = 0.25;
    qreal dataYMin = static_cast<qreal>(min_Y);
    qreal dataYMax = static_cast<qreal>(max_Y);
    qreal dataYRange = dataYMax - dataYMin;

    qreal paddingY = std::max(1.0, dataYRange * yPaddingFactor);
    qreal desiredYMin = dataYMin - paddingY;
    qreal desiredYMax = dataYMax + paddingY;

    qreal currentYMin = m_yAxis->min();
    qreal currentYMax = m_yAxis->max();
    qreal currentYRange = currentYMax - currentYMin;

    bool adjustY = false;
    qreal hysteresis = std::max(1.0, currentYRange * 0.05);  // 5% hysteresis

    if (desiredYMin < currentYMin - hysteresis || desiredYMax > currentYMax + hysteresis) {
        adjustY = true;
    }
    else if (currentYRange > 5.0 * dataYRange && dataYRange > 0) {
        adjustY = true;
    }

    if (adjustY) {
        qreal roundTo = std::max(1.0, std::pow(10, std::floor(std::log10(dataYRange / 5))));
        qreal newYMin = std::floor(desiredYMin / roundTo) * roundTo;
        qreal newYMax = std::ceil(desiredYMax / roundTo) * roundTo;

        if (newYMin > 0 && newYMin < dataYRange * 0.5) newYMin = 0;
        if (newYMax < 0 && newYMax > -dataYRange * 0.5) newYMax = 0;

        if (newYMax - newYMin < 1.0) {
            qreal midPoint = (newYMax + newYMin) / 2.0;
            newYMin = midPoint - 0.5;
            newYMax = midPoint + 0.5;
        }

        m_yAxis->setRange(newYMin, newYMax);

        int tickCount = std::min(11, std::max(5, static_cast<int>((newYMax - newYMin) / roundTo) + 1));
        m_yAxis->setTickCount(tickCount);
        m_yAxis->applyNiceNumbers();
    }
}

auto PathChart::InitializeAxis() -> void
{
    addAxis(m_xAxisTime, Qt::AlignBottom);
    addAxis(m_yAxis, Qt::AlignLeft);
    m_xAxisTime->setTitleText("Time (s)");
    m_yAxis->setTitleText("X<sub>t</sub>");
    m_xAxisTime->setGridLineVisible(false);
    m_yAxis->setGridLineVisible(false);
    m_xAxisTime->setRange(0, 10);
    m_yAxis->setRange(-10, 10);
}

auto PathChart::InitializeProcessChart() -> void
{
    GUI::SetChartStyle(this);
    legend()->setVisible(false);
    m_zeroLine->setPen(QPen(Qt::gray, 1.0));
    addSeries(m_zeroLine);
    m_zeroLine->attachAxis(m_xAxisTime);
    m_zeroLine->attachAxis(m_yAxis);
    m_zeroLine->append(0, 0);
    m_zeroLine->append(10, 0); // Match initial m_xAxisTime range
    m_driftCurve->setPen(QPen(Qt::gray, 1.0));
    addSeries(m_driftCurve);
    m_driftCurve->attachAxis(m_xAxisTime);
    m_driftCurve->attachAxis(m_yAxis);
    GUI::SetDriftStyle(m_driftCurve);
    setMargins(QMargins(50, 20, 20, 20));
}
