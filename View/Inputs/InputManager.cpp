#include "ButtonWidgetManager.hpp"
#include "ParametersWidget.hpp"
#include "ProcessButtonsManager.hpp"
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>

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
    mainLayout->addWidget(new QPushButton("Clear", this));
    mainLayout->addLayout(m_parametersWidget->layout());
    mainLayout->addLayout(m_ProcessButtonsManager->layout());
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
}

auto InputManager::SetListener(const InputHandler* inputHandler) -> void
{
    m_inputHandler = inputHandler;
}

