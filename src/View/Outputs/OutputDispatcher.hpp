#pragma once

#include "Types.hpp"
#include "PathChart.hpp"
#include "DistributionChart.hpp"
#include "StatusManager.hpp"
#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>

struct PathQuery;

class OutputDispatcher : public QWidget{
    Q_OBJECT

public:
    explicit OutputDispatcher(QWidget* parent = nullptr);

    // StatusManager
    void setQueryInfo(const PathQuery& query) const{
        m_statusManager->setQueryInfo(query);
    }
    void setProgress(size_t pathsCompleted) const{
        m_statusManager->setProgress(pathsCompleted);
    }
    void setResults(State minXT, State maxXT, State minXt, State maxXt) const {
        m_statusManager->setResults(minXT, maxXT, minXt, maxXt);
    }
    void setEVSTDInfo(State mu, State sigma) const{
        m_statusManager->setEVSTDInfo(mu, sigma);
    }
    void prepareStatusInfo(const size_t totalPaths) const{
        m_statusManager->prepareStatusInfo(totalPaths);
    }
    void setReady() const{
        m_statusManager->setReady();
    }
    void clearStatusBar() const{
        m_statusManager->clear();
    }
    void cancelStatusBar() const{
        m_statusManager->cancel();
    }
    StatusManager* getStatusManager() const {
        return m_statusManager;
    }

    // PathChart
    void plotPaths(Paths&& paths) const{
        getPathChart()->plotPaths(std::move(paths));
    }
    void updatePathChartTitle(bool allPathsPlotted) const{
        getPathChart()->updateTitle(allPathsPlotted);
    }
    void plotPathChartDriftData(const Path &drift, State minXt, State maxXt) const{
        getPathChart()->plotDriftCurve(drift, minXt, maxXt);
    }
    void clearPathChart(){
        getPathChart()->clearPathChart();
    }
    void setPathChartMaxTime(const Time time){
        getPathChart()->setMaxTime(time);
    }
    void expandYAxisRange(const State min, const State max) const {
        getPathChart()->expandYAxisRange(min, max);
    }

    // DistributionChart
    void updateDistributionChartTitle(const ProcessType t) const {
        getDistributionChart()->updateTitle(t);
    }
    void plotDistribution(const Distribution& distribution) const {
        getDistributionChart()->plotDistribution(distribution);
    }
    void updateDistributionChartPDF(const PDFData& pdfData) const {
        getDistributionChart()->updateDistributionChartPDF(pdfData);
    }
    void setDistributionChartSupport(const Range pdfDomain) const {
        getDistributionChart()->setDistributionChartSupport(pdfDomain);
    }
    void updateDistributionChartEV(const State EV) const {
        getDistributionChart()->plotExpValLine(EV);
    }
    void clearDistributionChart() {
        getDistributionChart()->clearDistributionChart();
    }

private:
    PathChart* getPathChart() const {
        return static_cast<PathChart*>(m_pathChartView->chart());
    }
    DistributionChart* getDistributionChart() const {
        return static_cast<DistributionChart*>(m_distributionChartView->chart());
    }
private:
    QVBoxLayout* layout;
    StatusManager* m_statusManager;
    QChartView* m_pathChartView;
    QChartView* m_distributionChartView;
};
