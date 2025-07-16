#include "InputDispatcher.hpp"
#include "ActionManager.hpp"
#include "DefinitionManager.hpp"
#include "SimulationManager.hpp"
#include "SettingsManager.hpp"
#include "InputHandler.hpp"
#include "MainWindow.hpp"
#include <QtWidgets/QBoxLayout>


InputDispatcher::InputDispatcher(QWidget* parent)
    : QWidget(parent)
    , m_actionManager(new ActionManager(this))
    , m_definitionManager(new DefinitionManager(this))
    , m_simulationManager(new SimulationManager(this))
    , m_settingsManager(new SettingsManager(this))
    , m_inputHandler(nullptr)
{
    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_actionManager);
    layout->addWidget(m_definitionManager);
    layout->addWidget(m_simulationManager);
    layout->addWidget(m_settingsManager);
}

void InputDispatcher::setInputHandler(InputHandler* inputHandler){
    m_inputHandler = inputHandler;
}

void InputDispatcher::onGoButtonClicked() const{
    m_inputHandler->samplePaths();
}

void InputDispatcher::onClearButtonClicked() const{
    m_inputHandler->clear();
}

void InputDispatcher::onCancelButtonClicked() const{
    m_inputHandler->cancel();
}

void InputDispatcher::onProcessTypeModified(const ProcessType newType) const{
    m_inputHandler->onProcessTypeModified(newType);
}

void InputDispatcher::onProcessDefinitionModified(const DefinitionWidget param, double userValue) const{
    m_inputHandler->onProcessDefinitionModified(param, userValue);
}

void InputDispatcher::onSolverTypeModified(const SolverType newType) const{
    m_inputHandler->onSolverTypeModified(newType);
}