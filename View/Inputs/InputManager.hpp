#pragma once
#include "ProcessData.hpp"
#include <QtWidgets/qwidget.h>


class ParametersManager;
class ProcessButtonsManager;
class InputHandler;

struct InputParameters {
    double mu;
    double sigma;
    double startValue;
};

class InputManager : public QWidget
{
    Q_OBJECT

public:
    explicit InputManager(QWidget* parent = nullptr);
    auto SetInputHandler(InputHandler* inputHandler) -> void;
    auto OnProcessButtonPressed(const ProcessType type) const -> void;
    auto OnParametersChanged(const InputParameters& type) const -> void;
    

private:
    auto InitializeLayout() -> void;

private:
    ParametersManager* m_parametersWidget;
    ProcessButtonsManager* m_ProcessButtonsManager;
    InputHandler* m_inputHandler;
};

