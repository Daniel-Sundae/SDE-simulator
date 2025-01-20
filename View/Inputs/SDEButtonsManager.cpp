#include "SDEButtonsManager.hpp"
#include "CreateSDE.hpp"
#include "Types.hpp"

SDEButtonsManager::SDEButtonsManager(ButtonWidgetManager* parent) :
    QWidget(parent), m_activeButton(nullptr)
{
    CreateSDEButtons();
    InitializeSDEButtonsManager();
    SetupConnections();
}

auto SDEButtonsManager::InitializeSDEButtonsManager() -> void
{
    auto* buttonLayout = new QGridLayout(this);
    buttonLayout->addWidget(m_buttons[SDEType::BM]);
    buttonLayout->addWidget(m_buttons[SDEType::GBM]);
    setLayout(buttonLayout);
}

auto SDEButtonsManager::CreateSDEButtons() -> void
{
    m_buttons[SDEType::BM] = std::make_unique<SDEButton>(this, SDEType::BM);
    m_buttons[SDEType::GBM] = std::make_unique<SDEButton>(this, SDEType::GBM);
}

auto SDEButtonsManager::SetupConnections() -> void
{
    
    for (const auto& [_, button] : m_buttons) {
        // Allow all buttons to signal chart
        connect(
            button,
            &SDEButton::RequestUpdatePathChart,
            this,
            &SDEButtonsManager::ForwardRequestUpdatePathChart
        );
        connect(
            button,
            &SDEButton::SetActive,
            this,
            &SDEButtonsManager::SetActive(button)
        );

    }
}

auto SDEButtonsManager::OnParametersReceived(const SDEDefinition& def) -> void
{
    if (m_activeButton)
    {
        m_activeButton->OnParametersReceived(def);
    }
}

auto SDEButtonsManager::SetActive(SDEButton* button) -> void
{
    m_activeButton = button;
}

auto SDEButtonsManager::ClearActive() -> void
{
    m_activeButton = nullptr;
}


