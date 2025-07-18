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
    void setInputHandler(InputHandler* inputHandler){ m_inputHandler = inputHandler; };
    void onGoButtonClicked() const{ m_inputHandler->samplePaths();}
    void onClearButtonClicked() const{ m_inputHandler->clear(); }
    void onCancelButtonClicked() const{ m_inputHandler->cancel(); }
    void onProcessTypeModified(const ProcessType newType) const{
        m_inputHandler->onProcessTypeModified(newType);
    }
    void onProcessDefinitionModified(const DefinitionWidget param, double userValue) const{
        m_inputHandler->onProcessDefinitionModified(param, userValue);
    }
    void onSolverTypeModified(const SolverType newType) const{
        m_inputHandler->onSolverTypeModified(newType);
    }
    template <typename... Args>
    void onSimulationParameterModified(Args&&... args) const {
      m_inputHandler->onSimulationParameterModified(std::forward<Args>(args)...);
    }
    template <typename... Args>
    void onSettingsParameterModified(Args&&... args) const {
      m_inputHandler->onSettingsParameterModified(std::forward<Args>(args)...);
    }

private:
    ActionManager* m_actionManager;
    DefinitionManager* m_definitionManager;
    SimulationManager* m_simulationManager;
    SettingsManager* m_settingsManager;
    InputHandler* m_inputHandler = nullptr;
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
