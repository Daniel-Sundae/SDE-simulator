#include "PathChart.hpp"
#include "ProcessData.hpp"
#include "Types.hpp"
#include "ViewUtils.hpp"
#include <QtCharts/qlineseries.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qvalueaxis.h>

PathChart::PathChart()
    : QChart()
    , m_xAxis(new QValueAxis(this))
    , m_yAxis(new QValueAxis(this))
    , m_zeroLine(new QLineSeries(this))
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

auto PathChart::ClearPaths() -> void
{
    for (QAbstractSeries* s : series()) {
        if (s != m_zeroLine) {
            removeSeries(s);
        }
    }
}

auto PathChart::PlotDriftLine(const Path& driftLine) -> void
{
    PlotPath(driftLine);
    GUI::SetDriftStyle(qobject_cast<QLineSeries*>(series().last()));
}

auto PathChart::PlotPath(const Path& path) -> void
{
    if (path.empty()) return;

    QLineSeries* series = new QLineSeries(this);
    QVector<QPointF> points;
    points.reserve(static_cast<qsizetype>(path.size()));
    for (size_t i = 0; i < path.size(); ++i) {
        points.append(QPointF(i, path[i]));
    }
    series->replace(points);
    addSeries(series);
    series->attachAxis(m_xAxis);
    series->attachAxis(m_yAxis);
    auto [min_it, max_it] = std::minmax_element(path.begin(), path.end());
    GUI::SetPathStyle(series);
    UpdateAxisIfNeeded(path.size(), *min_it, *max_it);
    m_zeroLine->clear();
    m_zeroLine->append(m_xAxis->min(), 0);
    m_zeroLine->append(m_xAxis->max(), 0);
}

auto PathChart::UpdateAxisIfNeeded(std::size_t max_X, State min_Y, State max_Y) -> void
{
    // ---------- X-Axis Range Handling ----------

    qreal desiredXMax = static_cast<qreal>(max_X);
    qreal currentXMax = m_xAxis->max();

    bool adjustX = false;
    qreal newXMax = currentXMax;

    if (desiredXMax > currentXMax) {
        newXMax = std::ceil(desiredXMax / 10.0) * 10;
        adjustX = true;
    }
    else if (currentXMax > 3.0 * desiredXMax) {
        newXMax = std::ceil(desiredXMax / 10.0) * 10;
        adjustX = true;
    }

    if (adjustX) {
        m_xAxis->setRange(0, newXMax);
        int tickCount = std::min(11, std::max(5, static_cast<int>(newXMax / 20.0) + 1));
        m_xAxis->setTickCount(tickCount);
    }

    // ---------- Y-Axis Range Handling ----------
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
    }
}

auto PathChart::InitializeAxis() -> void
{
    addAxis(m_xAxis, Qt::AlignBottom);
    addAxis(m_yAxis, Qt::AlignLeft);
    m_xAxis->setTitleText("t (s)");
    m_yAxis->setTitleText("X<sub>t</sub>");
    m_xAxis->setGridLineVisible(false);
    m_yAxis->setGridLineVisible(false);
    m_xAxis->setRange(0, 10);
    m_yAxis->setRange(-10, 10);
}

auto PathChart::InitializeProcessChart() -> void
{
    GUI::SetChartStyle(this);
    legend()->setVisible(false);
    m_zeroLine->setPen(QPen(Qt::gray, 1.0));
    addSeries(m_zeroLine);
    m_zeroLine->attachAxis(m_xAxis);
    m_zeroLine->attachAxis(m_yAxis);
    m_zeroLine->append(0, 0);
    m_zeroLine->append(10, 0); // Match initial m_xAxis range
}

