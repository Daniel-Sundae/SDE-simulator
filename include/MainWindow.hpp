#pragma once

#include "ChartWidget.hpp"
#include "ButtonWidget.hpp"
#include <QtWidgets/qmainwindow.h>
#include <QVBoxLayout>

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow();
    
private:
    void InitializeMainWindow();

    QHBoxLayout* m_mainWindowLayout;
    ChartWidget* m_chartWidget;
    ButtonWidget* m_buttonWidget;
};

