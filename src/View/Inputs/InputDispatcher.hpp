#pragma once
#include "ProcessData.hpp"
#include "InputHandler.hpp"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>

class ActionManager;
class DefinitionManager;
class SimulationManager;
class SettingsManager;
class InputHandler;

class InputDispatcher : public QWidget
{
    Q_OBJECT

public:
    explicit InputDispatcher(QWidget* parent);
    void setInputHandler(InputHandler* inputHandler);
    void onGoButtonClicked() const;
    void onClearButtonClicked() const;
    void onCancelButtonClicked() const;
    void onProcessTypeModified(const ProcessType newType) const;
    void onProcessDefinitionModified(const DefinitionWidget param, double userValue) const;
    void onSolverTypeModified(const SolverType newType) const;
    template<IntOrDouble T> // TODO: Remove?
    void onSimulationParameterModified(const SimulationWidget param, T userValue) const{
        m_inputHandler->onSimulationParameterModified(param, userValue);
    }
    template<IntOrBool T> // TODO: Remove?
    void onSettingsParameterModified(const SettingsWidget param, T userValue) const{
        m_inputHandler->onSettingsParameterModified(param, userValue);
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
