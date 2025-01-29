#pragma once

#include "ChartWidget.hpp"
#include "ButtonWidgetManager.hpp"
#include <QtWidgets/qmainwindow.h>
#include <QVBoxLayout>

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow();
    
private:
    auto InitializeMainWindow() -> void;

    auto SetupConnections() const -> void;

    QHBoxLayout* m_mainWindowLayout;
    ChartWidget* m_chartWidget;
    InputManager* m_buttonWidget;
};

