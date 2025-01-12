#include "ButtonWidgetManager.hpp"
#include "ParametersWidget.hpp"
#include "SDEButtonsWidget.hpp"
#include <QtWidgets/qlabel.h>

ButtonWidgetManager::ButtonWidgetManager(QWidget* parent):
    QWidget(parent),
    m_parametersWidget(new ParametersWidget(this)),
    m_SDEButtonsWidget(new SDEButtonsWidget(this))
{
    InitializeLayout();
    ConnectSignals();
}
//---------------------------------------------------------------------------//
auto ButtonWidgetManager::InitializeLayout() -> void
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QPushButton("Clear", this));
    mainLayout->addLayout(m_parametersWidget->layout());
    mainLayout->addLayout(m_SDEButtonsWidget->layout());
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
}
//---------------------------------------------------------------------------//
auto ButtonWidgetManager::ConnectSignals() -> void
{

}