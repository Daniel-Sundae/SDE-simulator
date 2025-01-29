#include "ParametersManager.hpp"
#include <QtWidgets/qlabel.h>

ParametersManager::ParametersManager(InputManager* parent, std::shared_ptr<InputPresenter> parameterPresenter)
    : QWidget(parent)
    , m_parameterPresenter(std::move(parameterPresenter))
{
	InitializeParametersManager();
}

auto ParametersManager::InitializeParametersWidget() -> void
{
    auto* layout = new QHBoxLayout(this);

    auto* muInput = new QDoubleSpinBox(this);
    muInput->setObjectName("muValue");
    muInput->setRange(-10.0, 10.0);
    muInput->setValue(0.2);
    muInput->setSingleStep(0.1);
    layout->addWidget(new QLabel("Drift (μ):"));
    layout->addWidget(muInput);

    auto* sigmaInput = new QDoubleSpinBox(this);
    sigmaInput->setObjectName("sigmaValue");
    sigmaInput->setRange(0.0, 10.0);
    sigmaInput->setValue(0.2);
    sigmaInput->setSingleStep(0.1);
    layout->addWidget(new QLabel("Volatility (σ):"));
    layout->addWidget(sigmaInput);

    auto* startValueInput = new QDoubleSpinBox(this);
    startValueInput->setObjectName("startValue");
    startValueInput->setRange(-20, 20.0);
    startValueInput->setValue(0);
    startValueInput->setSingleStep(1);
    layout->addWidget(new QLabel("Start Value:"));
    layout->addWidget(startValueInput);
    setLayout(layout);
}

auto ParametersManager::GetMuValue() const -> double
{
    auto* muInput = findChild<QDoubleSpinBox*>("muValue");
    return muInput->value();
}

auto ParametersManager::GetSigmaValue() const -> double
{
    auto* sigmaInput = findChild<QDoubleSpinBox*>("sigmaValue");
    return sigmaInput->value();
}

auto ParametersManager::GetStartValue() const -> double
{
    auto* startValueInput = findChild<QDoubleSpinBox*>("startValue");
    return startValueInput->value();
}

auto ParametersManager::OnParametersChanged() -> void
{
    // def.mu = GetMuValue();
    // def.sigma = GetSigmaValue();
    // def.startValue = GetStartValue();
    m_listener->OnParametersChanged(ProcessDefinition{GetMuValue(), GetSigmaValue(), GetStartValue()});
}