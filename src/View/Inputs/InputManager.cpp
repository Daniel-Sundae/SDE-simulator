#include "InputManager.hpp"
#include "DefinitionManager.hpp"
#include "SimulationManager.hpp"
#include "InputHandler.hpp"
#include "MainWindow.hpp"
#include <QtWidgets/qboxlayout.h>


InputManager::InputManager(QWidget* parent)
    : QWidget(parent)
    , m_definitionManager(new DefinitionManager(this))
    , m_simulationManager(new SimulationManager(this))
    , m_inputHandler(nullptr)
{
    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_definitionManager);
    layout->addWidget(m_simulationManager);
}

auto InputManager::SetInputHandler(InputHandler* inputHandler) -> void
{
    m_inputHandler = inputHandler;
}

auto InputManager::OnProcessTypeModified(ProcessType newType) const -> void
{
    m_inputHandler->OnProcessTypeModified(newType);
}

auto InputManager::OnProcessDefinitionModified(const ModifiedDefinitionParam param, double userValue) const -> void
{
    m_inputHandler->OnProcessDefinitionModified(param, userValue);
}