#include "ProcessButtonsManager.hpp"
#include "DefinitionManager.hpp"
#include "InputHandler.hpp"
#include "InputManager.hpp"
#include "MainWindow.hpp"
#include <QtWidgets/qboxlayout.h>


InputManager::InputManager(QWidget* parent)
    : QWidget(parent)
    , m_definitionManager(new DefinitionManager(this))
    , m_processButtonsManager(new ProcessButtonsManager(this))
    , m_inputHandler(nullptr)
{
    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_definitionManager, 2);
    layout->addWidget(m_processButtonsManager, 6);
}

auto InputManager::SetInputHandler(InputHandler* inputHandler) -> void
{
    m_inputHandler = inputHandler;
}

auto InputManager::OnProcessButtonPressed(const ProcessType type) const -> void
{
    m_inputHandler->OnProcessButtonPressed(type);
}

auto InputManager::OnProcessDefinitionModified(const ModifiedDefinitionParam param, double userValue) const -> void
{
    m_inputHandler->OnProcessDefinitionModified(param, userValue);
}