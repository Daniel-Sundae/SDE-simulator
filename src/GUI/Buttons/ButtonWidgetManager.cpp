#include "ButtonWidgetManager.hpp"
#include "ParametersWidget.hpp"
#include "SDEButtonsManager.hpp"
#include <QtWidgets/qlabel.h>

ButtonWidgetManager::ButtonWidgetManager(QWidget* parent):
    QWidget(parent),
    m_parametersWidget(new ParametersWidget(this)),
    m_SDEButtonsManager(new SDEButtonsManager(this))
{
    InitializeLayout();
    SetupConnections();
}

auto ButtonWidgetManager::InitializeLayout() -> void
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QPushButton("Clear", this));
    mainLayout->addLayout(m_parametersWidget->layout());
    mainLayout->addLayout(m_SDEButtonsManager->layout());
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
}

auto ButtonWidgetManager::SetupConnections() -> void
{
    // Button signals chart to redraw
    connect(
        m_SDEButtonsManager,
        &SDEButtonsManager::ForwardRequestUpdatePathChart,
        this,
        &ButtonWidgetManager::ForwardRequestUpdatePathChart
    );

    // SDE parameters sent to button
    connect(
        m_parametersWidget,
        &ParametersWidget::EmitParameters,
        m_SDEButtonsManager,
        &SDEButtonsManager::OnParametersReceived
    );

    // Button asks for new parameters
    connect(
        m_SDEButtonsManager,
        &SDEButtonsManager::RequestParameters,
        m_parametersWidget,
        &ParametersWidget::RequestParameters
    );
}

