#pragma once

#include <QtWidgets/QMainWindow>

class QWidget;
class QHBoxLayout;
class InputDispatcher;
class OutputDispatcher;

class MainWindow final : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow();
    auto GetInputDispatcher() const -> InputDispatcher*;
    auto GetOutputDispatcher() const -> OutputDispatcher*;
private:
    auto AddWidgets() -> void;
private:
    QWidget* m_centralWidget;
    QHBoxLayout* m_mainWindowLayout;
    OutputDispatcher* m_outputDispatcher;
    InputDispatcher* m_inputDispatcher;
};
