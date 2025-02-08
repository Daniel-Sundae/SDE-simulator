#pragma once

#include "ChartManager.hpp"
#include "InputManager.hpp"
#include "QtWidgets\qmainwindow.h"
#include "QtWidgets\qboxlayout.h>

class MainWindow final : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow();
private:
    auto AddWidgets() -> void;
private:
    QHBoxLayout* m_mainWindowLayout;
    const ChartManager* m_chartManager;
    const InputManager* m_inputManager;
};

