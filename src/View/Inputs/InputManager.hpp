#pragma once
#include "ProcessData.hpp"
#include "InputHandler.hpp"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qwidget.h>

class ActionManager;
class DefinitionManager;
class SimulationManager;
class SettingsManager;
class InputHandler;

class InputManager : public QWidget
{
    Q_OBJECT

public:
    explicit InputManager(QWidget* parent = nullptr);
    auto SetInputHandler(InputHandler* inputHandler) -> void;
    auto OnProcessTypeModified(ProcessType newType) const -> void;
    auto OnProcessDefinitionModified(const DefinitionWidget param, double userValue) const -> void;
    auto OnGoButtonClicked() const -> void;
    auto OnClearButtonClicked() const -> void;

private:
    ActionManager* m_actionManager;
    DefinitionManager* m_definitionManager;
    SimulationManager* m_simulationManager;
    SettingsManager* m_settingsManager;
    InputHandler* m_inputHandler;
};

