#include "PathChart.hpp"
#include "ProcessData.hpp"
#include "Types.hpp"
#include "ViewUtils.hpp"
#include <QtCharts/qlineseries.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qvalueaxis.h>

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

auto PathChart::UpdateTitle(const PathQuery& pQuery) -> void
{
    QString title;
    QTextStream(&title)
        << "Definition: "
        << QString::fromUtf8(ProcessData::GetName(pQuery.processDefinition.type))
        << ": "
        << QString::fromUtf8(ProcessData::GetDefinition(pQuery.processDefinition.type))
        << " with μ = "
        << QString::number(pQuery.processDefinition.drift.Mu())
        << ", σ = "
        << QString::number(pQuery.processDefinition.diffusion.Sigma())
        << ", X<sub>0</sub> = "
        << QString::number(pQuery.processDefinition.startValueData)
        << '\n'
        << "Simulation: Time = "
        << QString::number(pQuery.simulationParameters.time)
        << ", dt = "
        << QString::number(pQuery.simulationParameters.dt)
        ;
    setTitle(title);
}

auto PathChart::ClearPathChart() -> void
{
    for (QAbstractSeries* s : series()) {
        if (s != m_zeroLine && s != m_driftCurve) {
            removeSeries(s);
            delete s;
        }
    }
}

auto PathChart::PlotDriftCurve(const Path& driftLine) -> void
{
    QVector<QPointF> points;
    points.reserve(static_cast<qsizetype>(driftLine.size()));
    const double intervalWidth = (m_xAxisTime->max() - m_xAxisTime->min())/driftLine.size();
    for (size_t i = 0; i < driftLine.size(); ++i) {
        points.append(QPointF(i*intervalWidth, driftLine[i]));
    }
    m_driftCurve->replace(points);    
}

auto PathChart::PlotPath(const Path& path) -> void
{
    if (path.empty()) return;

    QLineSeries* series = new QLineSeries(this);
    addSeries(series);
    QVector<QPointF> points;
    points.reserve(static_cast<qsizetype>(path.size()));
    const double intervalWidth = (m_xAxisTime->max() - m_xAxisTime->min())/path.size();
    for (size_t i = 0; i < path.size(); ++i) {
        points.append(QPointF(i*intervalWidth , path[i]));
    }
    series->replace(points);
    series->attachAxis(m_xAxisTime);
    series->attachAxis(m_yAxis);
    auto [min_it, max_it] = std::minmax_element(path.begin(), path.end());
    GUI::SetPathStyle(series);
    UpdateYAxisIfNeeded(*min_it, *max_it);
    UpdateZeroLine();
}

auto PathChart::SetMaxTime(const Time time) -> void
{
    m_xAxisTime->setRange(0, time);
}

auto PathChart::UpdateZeroLine() -> void
{
    m_zeroLine->clear();
    m_zeroLine->append(m_xAxisTime->min(), 0);
    m_zeroLine->append(m_xAxisTime->max(), 0);
}

auto PathChart::UpdateYAxisIfNeeded(State min_Y, State max_Y) -> void
{
    constexpr qreal yPaddingFactor = 0.15;  // 15% padding for Y
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

