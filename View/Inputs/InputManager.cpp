#include "ProcessButtonsManager.hpp"
#include "ParametersManager.hpp"
#include "InputHandler.hpp"
//#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>
#include "InputManager.hpp"

InputManager::InputManager(QWidget* parent)
    : QWidget(parent)
{
    auto m_parametersWidget = new ParametersManager(this, inputPresenter);
    auto m_ProcessButtonsManager = new ProcessButtonsManager(this, inputPresenter);

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

