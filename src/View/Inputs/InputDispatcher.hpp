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

class InputDispatcher : public QWidget
{
    Q_OBJECT

public:
    explicit InputDispatcher(QWidget* parent = nullptr);
    auto SetInputHandler(InputHandler* inputHandler) -> void;
    auto OnGoButtonClicked() const -> void;
    auto OnClearButtonClicked() const -> void;
    auto OnProcessTypeModified(const ProcessType newType) const -> void;
    auto OnProcessDefinitionModified(const DefinitionWidget param, double userValue) const -> void;
    auto OnSolverTypeModified(const SolverType newType) const -> void;
    template<IntOrDouble T>
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

class InputDispatcherGroupBox : public QGroupBox {
public:
    explicit InputDispatcherGroupBox(QWidget* parent = nullptr)
        : QGroupBox(parent)
    {}

    InputDispatcher* Parent() const {
        return qobject_cast<InputDispatcher*>(parent());
    }
};