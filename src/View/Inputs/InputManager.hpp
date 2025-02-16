#pragma once
#include "ProcessData.hpp"
#include "InputHandler.hpp"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qwidget.h>

class DefinitionManager;
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
    auto OnProcessDefinitionModified(const ModifiedDefinitionParam param, double userValue) const -> void;

private:
    DefinitionManager* m_definitionManager;
    ProcessButtonsManager* m_processButtonsManager;
    InputHandler* m_inputHandler;
};

