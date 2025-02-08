#pragma once
#include "ProcessMetaData.hpp"
#include <QtWidgets/qwidget.h>


class ParametersManager;
class ProcessButtonsManager;
class InputHandler;

class InputManager : public QWidget
{
    Q_OBJECT

public:
    explicit InputManager(QWidget* parent = nullptr);
    auto SetListener(const InputHandler* inputHandler) -> void;

slots:
    void ForwardRequestUpdatePathChart(
        const ProcessType type);

private:
    auto InitializeLayout() -> void;

private:
    ParametersManager* m_parametersWidget;
    ProcessButtonsManager* m_ProcessButtonsManager;
    const InputHandler* m_listener;
};

