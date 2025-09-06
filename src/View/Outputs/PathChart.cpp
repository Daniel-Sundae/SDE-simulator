#include "PathChart.hpp"
#include "ProcessData.hpp"
#include "Types.hpp"
#include "ViewUtils.hpp"
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtConcurrent/QtConcurrent>

static constexpr double defaultYAxisMin = -0.1;
static constexpr double defaultYAxisMax = 0.1;
static constexpr int maxPointsPerPath = 2000;

PathChart::PathChart()
    : QChart()
    , m_xAxisTime(new QValueAxis(this))
    , m_yAxis(new QValueAxis(this))
    , m_zeroLine(new QLineSeries(this))
    , m_driftCurve(new QLineSeries(this))
{
    initializeAxis();
    initializePathChart();
}

void PathChart::updateTitle(bool allPathsPlotted){
    QString title = allPathsPlotted ? "" : QString("Only plotting first %1 paths").arg(DefaultConstants::maxPathsToDraw);
    setTitle(title);
}

void PathChart::clearPathChart(){
    for (QAbstractSeries* s : series()) {
        if (s != m_zeroLine && s != m_driftCurve) {
            removeSeries(s);
            delete s;
        }
    }
    m_driftCurve->clear();
    m_zeroLine->clear();
    m_zeroLine->append(0, 0);
    m_zeroLine->append(DefaultConstants::Simulation::time, 0);
    m_yAxis->setRange(defaultYAxisMin, defaultYAxisMax);
    m_xAxisTime->setRange(0, DefaultConstants::Simulation::time);
}

void PathChart::plotDriftCurve(const Path& drift, State minXt, State maxXt){
    if(drift.size() < 2) return;
    QVector<QPointF> points;
    expandYAxisRange(minXt, maxXt);
    points.reserve(static_cast<qsizetype>(drift.size()));
    const double intervalWidth = (m_xAxisTime->max() - m_xAxisTime->min())/static_cast<qreal>(drift.size()-1);
    for (size_t i = 0; i < drift.size(); ++i) {
        points.append(QPointF(double(i)*intervalWidth, drift[i]));
    }
    m_driftCurve->replace(points);
}

static QVector<QPointF> pathToQVector(const Path& path, double xMin, double xMax){
    const size_t n = path.size();
    QVector<QPointF> points;
    if (n == 0) return points;
    if (n <= static_cast<size_t>(maxPointsPerPath)) {
        points.resize(static_cast<int>(n));
        const double dx = (n > 1) ? (xMax - xMin) / double(n - 1) : 0.0;
        for (size_t i = 0; i < n; ++i)
            points[i] = QPointF(xMin + i*dx, path[i]);
    }
    // Algorithm for downsampling to not choke the GUI thread when rendering
    else {
        points.reserve(maxPointsPerPath);
        const double dx = (xMax - xMin) / double(n - 1);
        points.append(QPointF(xMin, path[0]));
        const double bucketSize = double(n - 2) / double(maxPointsPerPath - 2);
        size_t a = 0;

        for (int i = 0; i < maxPointsPerPath - 2; ++i) {
            // current bucket [start, end)
            const size_t start = 1 + size_t(std::floor(i * bucketSize));
            size_t end = 1 + size_t(std::floor((i + 1) * bucketSize));
            if (end >= n - 1) end = n - 1;
            const size_t left = (start < end ? start : end); // guard degeneracy
            const size_t right = (start < end ? end : start + 1); // ensure at least one

            // mean of next bucket (for triangle anchor)
            size_t nextStart = 1 + size_t(std::floor((i + 1) * bucketSize));
            size_t nextEnd   = 1 + size_t(std::floor((i + 2) * bucketSize));
            if (nextEnd > n) nextEnd = n;
            double avgX = 0.0, avgY = 0.0;
            size_t cnt = (nextEnd > nextStart) ? (nextEnd - nextStart) : 1;
            if (cnt == 1) { avgX = xMin + (n - 1) * dx; avgY = path[n - 1]; }
            else {
                for (size_t j = nextStart; j < nextEnd; ++j) {
                    avgX += xMin + j*dx;
                    avgY += path[j];
                }
                avgX /= double(cnt);
                avgY /= double(cnt);
            }
            const double ax = xMin + a*dx;
            const double ay = path[a];
            double bestArea = -1.0;
            size_t bestIdx = left;
            for (size_t j = left; j < right; ++j) {
                const double bx = xMin + j*dx;
                const double by = path[j];
                // area of triangle A-B-avg (scaled by 2, we only compare)
                const double area = std::abs((ax - avgX)*(by - ay) - (ax - bx)*(avgY - ay));
                if (area > bestArea) { bestArea = area; bestIdx = j; }
            }
            points.append(QPointF(xMin + bestIdx*dx, path[bestIdx]));
            a = bestIdx;
        }
        points.append(QPointF(xMin + (n - 1)*dx, path[n - 1]));
    }
    return points;
}

void PathChart::plotPaths(Paths&& paths){
    if (paths.empty()) return;
    // Threadpool captures pointer copy instead of data. Lifetime safety.
    auto pathsKeepAlive = std::make_shared<Paths>(std::move(paths));
    Time xMin = m_xAxisTime->min();
    Time xMax = m_xAxisTime->max();
    for (size_t i = 0; i < pathsKeepAlive->size(); ++i) {
        // Computational pre-processing off GUI thread
        QFuture<QVector<QPointF>> pointsFut = QtConcurrent::task([pathsKeepAlive](const Path& p, Time xMin, Time xMax) {
                return pathToQVector(p, xMin, xMax);
            })
            .withArguments(std::cref((*pathsKeepAlive)[i]), xMin, xMax)
            .spawn();
        // Rendering on GUI thread
        pointsFut.then(this, [this](QVector<QPointF> points) {
            plotPath(std::move(points));
        });
    }
}

void PathChart::plotPath(QVector<QPointF> points){
    if (points.empty()) return;
    auto* series = new QLineSeries(this);
    addSeries(series);
    series->replace(points);
    series->attachAxis(m_xAxisTime);
    series->attachAxis(m_yAxis);
    GUI::setPathStyle(series);
}

void PathChart::setMaxTime(const Time time){
    m_xAxisTime->setRange(0, time);
    m_zeroLine->clear();
    m_zeroLine->append(m_xAxisTime->min(), 0);
    m_zeroLine->append(m_xAxisTime->max(), 0);
}

void PathChart::expandYAxisRange(const State min, const State max){
    m_yAxis->setRange(std::min(min, m_yAxis->min()), std::max(max, m_yAxis->max()));
}

void PathChart::initializeAxis(){
    addAxis(m_xAxisTime, Qt::AlignBottom);
    m_xAxisTime->setTitleText("t");
    m_xAxisTime->setGridLineVisible(false);
    m_xAxisTime->setRange(0, DefaultConstants::Simulation::time);
    addAxis(m_yAxis, Qt::AlignLeft);
    m_yAxis->setTitleText("X<sub>t</sub>");
    m_yAxis->setGridLineVisible(false);
    m_yAxis->setRange(defaultYAxisMin, defaultYAxisMax);
    m_yAxis->setTickType(QValueAxis::TicksFixed);
    m_yAxis->setTickCount(7);
    m_yAxis->setTickAnchor(0.0);
}

void PathChart::initializePathChart(){
    GUI::setChartStyle(this);
    legend()->setVisible(false);

    m_zeroLine->setPen(QPen(Qt::gray, 1.0));
    addSeries(m_zeroLine);
    m_zeroLine->attachAxis(m_xAxisTime);
    m_zeroLine->attachAxis(m_yAxis);
    m_zeroLine->append(0, 0);
    m_zeroLine->append(DefaultConstants::Simulation::time, 0); // Match initial m_xAxisTime range

    m_driftCurve->setPen(QPen(Qt::gray, 1.0));
    addSeries(m_driftCurve);
    m_driftCurve->attachAxis(m_xAxisTime);
    m_driftCurve->attachAxis(m_yAxis);
    GUI::setDriftStyle(m_driftCurve);
    setMargins(QMargins(50, 20, 20, 20));
}
