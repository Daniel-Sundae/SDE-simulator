#pragma once

#include <QtWidgets/qmainwindow.h>

class QWidget;
class QHBoxLayout;
class InputManager;
class OutputManager;

class MainWindow final : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow();
    auto GetInputManager() const -> InputManager*;
    auto GetOutputManager() const -> OutputManager*;
private:
    auto AddWidgets() -> void;
private:
    QWidget* m_centralWidget;
    QHBoxLayout* m_mainWindowLayout;
    OutputManager* m_outputManager;
    InputManager* m_inputManager;
};

