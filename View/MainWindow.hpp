#pragma once

#include "ChartWidget.hpp"
#include "ButtonWidgetManager.hpp"
#include <QtWidgets/qmainwindow.h>
#include <QVBoxLayout>

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow();
    auto SetChartManager(ChartManager* chartManager) -> void;
    auto SetInputManager(InputManager* inputManager) -> void;
    auto AddWidgets() -> void;
    
private:

    QHBoxLayout* m_mainWindowLayout;
    ChartManager* m_chartManager;
    InputManager* m_inputManager;
    bool m_widgetsAdded;
};

