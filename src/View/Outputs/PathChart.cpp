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
    initializeAxis();
    initializeProcessChart();
}

void PathChart::updateTitle(bool allPathsPlotted){
    QString title = allPathsPlotted ? "" : QString("Only plotting first %1 paths").arg(DefaultConstants::maxPathsToDraw);
    setTitle(title);
}

void PathChart::clearPathChart(bool clearDrift){
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

void PathChart::plotDriftCurve(const Path& driftCurve){
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
    updateYAxisIfNeeded(min_y, max_y);
}

// This function is written by chatGPT.
// Removes points from plot to reduce clutter and increase rendering speed
static QVector<QPointF> lttbDownsampleUniform(const Path& y, int maxPoints, double xMin, double xMax) {
    const size_t n = y.size();
    QVector<QPointF> out;
    if (n == 0) return out;

    if (n <= static_cast<size_t>(maxPoints)) {
        out.resize(static_cast<int>(n));
        const double dx = (n > 1) ? (xMax - xMin) / double(n - 1) : 0.0; // note: n-1
        for (size_t i = 0; i < n; ++i)
            out[static_cast<int>(i)] = QPointF(xMin + i*dx, y[i]);
        return out;
    }

    out.reserve(maxPoints);

    const double dx = (xMax - xMin) / double(n - 1);
    out.append(QPointF(xMin, y[0]));

    const double bucketSize = double(n - 2) / double(maxPoints - 2);
    size_t a = 0;

    for (int i = 0; i < maxPoints - 2; ++i) {
        // current bucket [start, end)
        const size_t start = 1 + size_t(std::floor(i * bucketSize));
        size_t end = 1 + size_t(std::floor((i + 1) * bucketSize));
        if (end >= n - 1) end = n - 1;
        const size_t left = (start < end ? start : end);      // guard degeneracy
        const size_t right = (start < end ? end : start + 1); // ensure at least one

        // mean of next bucket (for triangle anchor)
        size_t nextStart = 1 + size_t(std::floor((i + 1) * bucketSize));
        size_t nextEnd   = 1 + size_t(std::floor((i + 2) * bucketSize));
        if (nextEnd > n) nextEnd = n;
        double avgX = 0.0, avgY = 0.0;
        size_t cnt = (nextEnd > nextStart) ? (nextEnd - nextStart) : 1;
        if (cnt == 1) { avgX = xMin + (n - 1) * dx; avgY = y[n - 1]; }
        else {
            for (size_t j = nextStart; j < nextEnd; ++j) {
                avgX += xMin + j*dx;
                avgY += y[j];
            }
            avgX /= double(cnt);
            avgY /= double(cnt);
        }

        const double ax = xMin + a*dx;
        const double ay = y[a];
        double bestArea = -1.0;
        size_t bestIdx = left;

        for (size_t j = left; j < right; ++j) {
            const double bx = xMin + j*dx;
            const double by = y[j];
            // area of triangle A-B-avg (scaled by 2, we only compare)
            const double area = std::abs((ax - avgX)*(by - ay) - (ax - bx)*(avgY - ay));
            if (area > bestArea) { bestArea = area; bestIdx = j; }
        }

        out.append(QPointF(xMin + bestIdx*dx, y[bestIdx]));
        a = bestIdx;
    }

    out.append(QPointF(xMin + (n - 1)*dx, y[n - 1]));
    return out;
}

void PathChart::plotPath(const Path& path){
    if (path.empty()) return;
    
    const double xMin = m_xAxisTime->min();
    const double xMax = m_xAxisTime->max();
    
    static constexpr int kMaxPoints = 2000;
    const QVector<QPointF> points = lttbDownsampleUniform(path, kMaxPoints, xMin, xMax);

    auto* series = new QLineSeries(this);
    addSeries(series);
    series->replace(points);
    series->attachAxis(m_xAxisTime);
    series->attachAxis(m_yAxis);
    GUI::setPathStyle(series);
}


void PathChart::setMaxTime(const Time time){
    m_xAxisTime->setRange(0, time);
    updateZeroLine();
}

void PathChart::updateZeroLine(){
    m_zeroLine->clear();
    m_zeroLine->append(m_xAxisTime->min(), 0);
    m_zeroLine->append(m_xAxisTime->max(), 0);
}

void PathChart::updateYAxisIfNeeded(State min_Y, State max_Y){
    constexpr qreal yPaddingFactor = 0.3;
    qreal dataYMin = static_cast<qreal>(min_Y);
    qreal dataYMax = static_cast<qreal>(max_Y);
    qreal dataYRange = dataYMax - dataYMin;

    qreal paddingY = std::max(0.3, dataYRange * yPaddingFactor);
    qreal desiredYMin = dataYMin - paddingY;
    qreal desiredYMax = dataYMax + paddingY;

    qreal currentYMin = m_yAxis->min();
    qreal currentYMax = m_yAxis->max();
    qreal currentYRange = currentYMax - currentYMin;
    bool adjustY = false;
    qreal hysteresis = std::max(0.5, currentYRange * 0.05);  // 5% hysteresis
    if (desiredYMin < currentYMin - hysteresis || desiredYMax > currentYMax + hysteresis) {
        adjustY = true;
    } else if (currentYRange > 5.0 * dataYRange && dataYRange > 0) {
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

void PathChart::initializeAxis(){
    addAxis(m_xAxisTime, Qt::AlignBottom);
    addAxis(m_yAxis, Qt::AlignLeft);
    m_xAxisTime->setTitleText("Time (s)");
    m_yAxis->setTitleText("X<sub>t</sub>");
    m_xAxisTime->setGridLineVisible(false);
    m_yAxis->setGridLineVisible(false);
    m_xAxisTime->setRange(0, 10);
    m_yAxis->setRange(-10, 10);
}

void PathChart::initializeProcessChart(){
    GUI::setChartStyle(this);
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
    GUI::setDriftStyle(m_driftCurve);
    setMargins(QMargins(50, 20, 20, 20));
}
