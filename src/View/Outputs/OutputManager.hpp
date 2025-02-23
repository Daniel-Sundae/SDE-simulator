#pragma once

#include "PathChart.hpp"
#include "Types.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qchart.h>


class OutputManager : public QWidget{
    Q_OBJECT

public:
    explicit OutputManager(QWidget* parent = nullptr);
    auto OnPathReceived(const PathQuery& query, const Path& sampleData) const -> void;
    auto Clear() -> void;
private:
    auto GetPathChart() const->PathChart*;
private:
    QVBoxLayout* m_layout;
    QChartView* m_pathChartView;
    QChartView* m_distributionChartView;
};