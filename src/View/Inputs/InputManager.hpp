#pragma once
#include "ProcessData.hpp"
#include "InputHandler.hpp"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qwidget.h>

class DefinitionManager;
class SimulationManager;
class InputHandler;

class InputManager : public QWidget
{
    Q_OBJECT

public:
    explicit InputManager(QWidget* parent = nullptr);
    auto SetInputHandler(InputHandler* inputHandler) -> void;
    auto OnProcessTypeModified(ProcessType newType) const -> void;
    auto OnProcessDefinitionModified(const ModifiedDefinitionParam param, double userValue) const -> void;

private:
    DefinitionManager* m_definitionManager;
    SimulationManager* m_simulationManager;
    InputHandler* m_inputHandler;
};

