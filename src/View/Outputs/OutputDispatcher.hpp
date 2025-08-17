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
    void setQueryInfo(const PathQuery& pQuery) const{
        m_statusManager->setQueryInfo(pQuery);
    }
    void setStatus(const StatusSignal signal) const{
        m_statusManager->setStatus(signal);
    }
    void setError(const ErrorType error) const{
        // m_statusManager->setError(error);
        int TODO = 1;
        (void)TODO;
    }
    void clearStatus() const{
        m_statusManager->clearStatus();
    }

    // PathChart
    void plotPath(const Path& path) const{
        getPathChart()->plotPath(path);
    }
    void updatePathChartTitle(bool allPathsPlotted) const{
        getPathChart()->updateTitle(allPathsPlotted);
    }
    void plotPathChartDriftData(const Path &driftLine) const{
        getPathChart()->plotDriftCurve(driftLine);
    }
    void clearPathChart(bool clearDrift = true){
        getPathChart()->clearPathChart(clearDrift);
    }
    void setPathChartMaxTime(const Time time){
        getPathChart()->setMaxTime(time);
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
