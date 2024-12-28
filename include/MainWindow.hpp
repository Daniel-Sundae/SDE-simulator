#pragma once

#include "ChartWidget.hpp"
#include "ButtonWidget.hpp"
#include <QtWidgets/qmainwindow.h>
#include <QVBoxLayout>

class MainWindow : public QMainWindow{
public:
    MainWindow();    
    
private:
    void InitializeMainWindow();

    QVBoxLayout* m_mainWindowLayout;
    ChartWidget* m_chartWidget;
    ButtonWidget* m_buttonWidget;
};

