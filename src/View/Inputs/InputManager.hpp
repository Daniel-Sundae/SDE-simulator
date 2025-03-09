#pragma once
#include "ProcessData.hpp"
#include "InputHandler.hpp"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qgroupbox.h>

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
    auto OnGoButtonClicked() const -> void;
    auto OnClearButtonClicked() const -> void;
    auto OnProcessTypeModified(const ProcessType newType) const -> void;
    auto OnProcessDefinitionModified(const DefinitionWidget param, double userValue) const -> void;
    auto OnSolverTypeModified(const SolverType newType) const -> void;
    template<UInt64OrDouble T>
    auto OnSimulationParametersModified(const SimulationWidget param, T userValue) const -> void {
        m_inputHandler->OnSimulationParametersModified(param, userValue);
    }

private:
    ActionManager* m_actionManager;
    DefinitionManager* m_definitionManager;
    SimulationManager* m_simulationManager;
    SettingsManager* m_settingsManager;
    InputHandler* m_inputHandler;
};

class InputManagerGroupBox : public QGroupBox {
public:
    InputManagerGroupBox(QWidget* parent = nullptr)
        : QGroupBox(parent)
    {}

    InputManager* Parent() const {
        return qobject_cast<InputManager*>(parent());
    }
};