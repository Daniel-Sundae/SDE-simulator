#pragma once

#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QVBoxLayout>
#include <functional>

class RunApplication {
public:
    RunApplication();
    
private:
    void InitializeMainWindow();
    void CreateDefaultButtons();
    void CreateButton(const QString &label, std::function<std::vector<double>()> sample);

    void PlotChart();

    QWidget m_mainWindow;
    QChart m_mainChart;
    QChartView m_mainChartView;
    QVBoxLayout m_layout;
};

