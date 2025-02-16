#include "ProcessButtonsManager.hpp"
#include "DefinitionManager.hpp"
#include "InputHandler.hpp"
#include <QtWidgets/qboxlayout.h>
#include "InputManager.hpp"

InputManager::InputManager(QWidget* parent)
    : QWidget(parent)
    , m_parametersWidget(new DefinitionManager(this))
    , m_ProcessButtonsManager(new ProcessButtonsManager(this))
{
    InitializeLayout();
}

auto InputManager::InitializeLayout() -> void
{
    auto* mainLayout = new QVBoxLayout(this);
    //mainLayout->addWidget(new QPushButton("Clear", this));
    mainLayout->addLayout(m_parametersWidget->layout());
    mainLayout->addLayout(m_ProcessButtonsManager->layout());
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
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