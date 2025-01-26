#include "ButtonWidgetManager.hpp"
#include "ParametersWidget.hpp"
#include "ProcessButtonsManager.hpp"
#include <QtWidgets/qlabel.h>

ButtonWidgetManager::ButtonWidgetManager(QWidget* parent):
    QWidget(parent),
    m_parametersWidget(new ParametersWidget(this)),
    m_ProcessButtonsManager(new ProcessButtonsManager(this))
{
    InitializeLayout();
    SetupConnections();
}

auto ButtonWidgetManager::InitializeLayout() -> void
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QPushButton("Clear", this));
    mainLayout->addLayout(m_parametersWidget->layout());
    mainLayout->addLayout(m_ProcessButtonsManager->layout());
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
}

auto ButtonWidgetManager::SetupConnections() -> void
{
    // Button signals chart to redraw
    connect(
        m_ProcessButtonsManager,
        &ProcessButtonsManager::ForwardRequestUpdatePathChart,
        this,
        &ButtonWidgetManager::ForwardRequestUpdatePathChart
    );

    // Process parameters sent to button
    connect(
        m_parametersWidget,
        &ParametersWidget::EmitParameters,
        m_ProcessButtonsManager,
        &ProcessButtonsManager::OnParametersReceived
    );

    // Button asks for new parameters
    connect(
        m_ProcessButtonsManager,
        &ProcessButtonsManager::RequestParameters,
        m_parametersWidget,
        &ParametersWidget::RequestParameters
    );
}

