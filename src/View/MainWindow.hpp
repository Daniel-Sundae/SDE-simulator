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
    InputDispatcher* getInputDispatcher() const;
    OutputDispatcher* getOutputDispatcher() const;
private:
    void addWidgets();
private:
    QWidget* m_centralWidget;
    QHBoxLayout* m_mainWindowLayout;
    OutputDispatcher* m_outputDispatcher;
    InputDispatcher* m_inputDispatcher;
};
