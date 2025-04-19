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

auto InputDispatcher::SetInputHandler(InputHandler* inputHandler) -> void
{
    m_inputHandler = inputHandler;
}

auto InputDispatcher::OnGoButtonClicked() const -> void
{
    m_inputHandler->SamplePaths();
}

auto InputDispatcher::OnClearButtonClicked() const -> void
{
    m_inputHandler->Clear();
}

auto InputDispatcher::OnCancelButtonClicked() const -> void
{
    m_inputHandler->Cancel();
}

auto InputDispatcher::OnProcessTypeModified(const ProcessType newType) const -> void
{
    m_inputHandler->OnProcessTypeModified(newType);
}

auto InputDispatcher::OnProcessDefinitionModified(const DefinitionWidget param, double userValue) const -> void
{
    m_inputHandler->OnProcessDefinitionModified(param, userValue);
}

auto InputDispatcher::OnSolverTypeModified(const SolverType newType) const -> void
{
    m_inputHandler->OnSolverTypeModified(newType);
}
