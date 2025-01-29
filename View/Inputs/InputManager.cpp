#include "ButtonWidgetManager.hpp"
#include "ParametersWidget.hpp"
#include "ProcessButtonsManager.hpp"
#include <QtWidgets/qlabel.h>

InputManager::InputManager(QWidget* parent)
    : QWidget(parent)
{
    auto inputPresenter = std::make_shared<InputPresenter>();
    auto m_parametersWidget = new ParametersManager(this, inputPresenter);
    auto m_ProcessButtonsManager = new ProcessButtonsManager(this, inputPresenter);

    InitializeLayout();
    SetupConnections();
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




// auto InputManager::SetupConnections() -> void
// {
//     // Button signals chart to redraw
//     connect(
//         m_ProcessButtonsManager,
//         &ProcessButtonsManager::ForwardRequestUpdatePathChart,
//         this,
//         &InputManager::ForwardRequestUpdatePathChart
//     );

//     // Process parameters sent to button
//     connect(
//         m_parametersWidget,
//         &ParametersManager::EmitParameters,
//         m_ProcessButtonsManager,
//         &ProcessButtonsManager::OnParametersReceived
//     );

//     // Button asks for new parameters
//     connect(
//         m_ProcessButtonsManager,
//         &ProcessButtonsManager::RequestParameters,
//         m_parametersWidget,
//         &ParametersManager::RequestParameters
//     );
// }

