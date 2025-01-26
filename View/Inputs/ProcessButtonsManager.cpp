#include "ProcessButtonsManager.hpp"
#include "CreateProcess.hpp"
#include "Types.hpp"

ProcessButtonsManager::ProcessButtonsManager(ButtonWidgetManager* parent) :
    QWidget(parent), m_activeButton(nullptr)
{
    CreateProcessButtons();
    InitializeProcessButtonsManager();
    SetupConnections();
}

auto ProcessButtonsManager::InitializeProcessButtonsManager() -> void
{
    auto* buttonLayout = new QGridLayout(this);
    buttonLayout->addWidget(m_buttons[ProcessType::BM]);
    buttonLayout->addWidget(m_buttons[ProcessType::GBM]);
    setLayout(buttonLayout);
}

auto ProcessButtonsManager::CreateProcessButtons() -> void
{
    m_buttons[ProcessType::BM] = std::make_unique<ProcessButton>(this, ProcessType::BM);
    m_buttons[ProcessType::GBM] = std::make_unique<ProcessButton>(this, ProcessType::GBM);
}


auto ProcessButtonsManager::SetupConnections() -> void
{
    
    for (const auto& [_, button] : m_buttons) {
        // Allow all buttons to signal chart
        connect(
            button,
            &ProcessButton::RequestUpdatePathChart,
            this,
            &ProcessButtonsManager::ForwardRequestUpdatePathChart
        );
        connect(
            button,
            &ProcessButton::SetActive,
            this,
            &ProcessButtonsManager::SetActive(button)
        );

    }
}

auto ProcessButtonsManager::OnParametersReceived(const ProcesProcessfinition& def) -> void
{
    if (m_activeButton)
    {
        m_activeButton->OnParametersReceived(def);
    }
}

auto ProcessButtonsManager::SetActive(ProcessButton* button) -> void
{
    m_activeButton = button;
}

auto ProcessButtonsManager::ClearActive() -> void
{
    m_activeButton = nullptr;
}


