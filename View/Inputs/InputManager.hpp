#pragma once
#include "ProcessMetaData.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>

class ParametersManager;
class ProcessButtonsManager;

class InputManager : public QWidget
{
    Q_OBJECT

public:
    explicit InputManager(QWidget* parent = nullptr);

slots:
    void ForwardRequestUpdatePathChart(
        const ProcessType type);

private:
    auto InitializeLayout() -> void;
    auto SetupConnections() -> void;

private:
    ParametersManager* m_parametersWidget;
    ProcessButtonsManager* m_ProcessButtonsManager;
};

