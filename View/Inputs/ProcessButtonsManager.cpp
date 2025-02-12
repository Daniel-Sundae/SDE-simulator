#include "ProcessButtonsManager.hpp"
#include "InputManager.hpp"
#include "ProcessButton.hpp"
#include <QtWidgets/qgridlayout.h>

ProcessButtonsManager::ProcessButtonsManager(InputManager* parent) :
    QWidget(parent)
{
    InitializeProcessButtonsManager();
}

auto ProcessButtonsManager::InitializeProcessButtonsManager() -> void
{
    auto* buttonLayout = new QGridLayout(this);
    m_buttons[ProcessType::BM] = new ProcessButton(this, ProcessType::BM);
    buttonLayout->addWidget(m_buttons[ProcessType::BM]);
    m_buttons[ProcessType::GBM] = new ProcessButton(this, ProcessType::GBM);
    buttonLayout->addWidget(m_buttons[ProcessType::GBM]);
    setLayout(buttonLayout);
}

auto ProcessButtonsManager::OnButtonPressed(ProcessType type) -> void
{
    static_cast<InputManager*>(parent())->OnProcessButtonPressed(type);
}



