#include "ProcessButtonsManager.hpp"
#include "Types.hpp"
#include "ButtonPresenter.hpp"

ProcessButtonsManager::ProcessButtonsManager(InputManager* parent, std::shared_pointer<InputPresenter> listener) :
    QWidget(parent), m_listener(std::move(listener))
{
    CreateProcessButtons();
    InitializeProcessButtonsManager();
}

auto ProcessButtonsManager::InitializeProcessButtonsManager() -> void
{
    auto* buttonLayout = new QGridLayout(this);
    m_buttons[ProcessType::BM] = std::make_unique<ProcessButton>(this, ProcessType::BM);
    buttonLayout->addWidget(m_buttons[ProcessType::BM]);
    m_buttons[ProcessType::GBM] = std::make_unique<ProcessButton>(this, ProcessType::GBM);
    buttonLayout->addWidget(m_buttons[ProcessType::GBM]);
    setLayout(buttonLayout);
}

auto ProcessButtonsManager::OnButtonPressed(ProcessType type) -> void
{
    m_listener->OnProcessButtonPressed(type);
}



