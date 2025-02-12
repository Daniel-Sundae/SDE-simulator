#include "ParametersManager.hpp"
#include "InputManager.hpp"
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qdoublespinbox>

ParametersManager::ParametersManager(InputManager* parent)
    : QWidget(parent)
{
	InitializeParametersManager();
}

auto ParametersManager::InitializeParametersManager() -> void
{
    auto* layout = new QHBoxLayout(this);

    auto* muInput = new QDoubleSpinBox(this);
    muInput->setRange(-10.0, 10.0);
    muInput->setValue(0.2);
    muInput->setSingleStep(0.1);
    m_inputs[InputType::MU] = muInput;
    layout->addWidget(new QLabel("Drift (μ):"));
    layout->addWidget(muInput);

    auto* sigmaInput = new QDoubleSpinBox(this);
    sigmaInput->setRange(0.0, 10.0);
    sigmaInput->setValue(0.2);
    sigmaInput->setSingleStep(0.1);
    m_inputs[InputType::SIGMA] = sigmaInput;
    layout->addWidget(new QLabel("Volatility (σ):"));
    layout->addWidget(sigmaInput);

    auto* startValueInput = new QDoubleSpinBox(this);
    startValueInput->setRange(-20, 20.0);
    startValueInput->setValue(0);
    startValueInput->setSingleStep(1);
    m_inputs[InputType::STARTVALUE] = startValueInput;
    layout->addWidget(new QLabel("Start Value:"));
    layout->addWidget(startValueInput);
    setLayout(layout);

    // TODO: WHat is this casting for?
    connect(
        muInput,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        &ParametersManager::OnParametersChanged);
    connect(
        sigmaInput,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        &ParametersManager::OnParametersChanged);
    connect(
        startValueInput,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        &ParametersManager::OnParametersChanged);
}

auto ParametersManager::GetMuValue() const -> double
{
    return m_inputs[InputType::MU]->value();
}

auto ParametersManager::GetSigmaValue() const -> double
{
    return m_inputs[InputType::SIGMA]->value();
}

auto ParametersManager::GetStartValue() const -> double
{
    return m_inputs[InputType::STARTVALUE]->value();
}

auto ParametersManager::Parent() const -> InputManager*
{
    return qobject_cast<InputManager*>(parent());
}

auto ParametersManager::OnParametersChanged() const -> void
{
    Parent()->OnParametersChanged(InputParameters{GetMuValue(), GetSigmaValue(), GetStartValue()});
}