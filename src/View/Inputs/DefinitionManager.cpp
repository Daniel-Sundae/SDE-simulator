#include "DefinitionManager.hpp"
#include "InputManager.hpp"
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qdoublespinbox>
#include "DefaultConstants.hpp"

DefinitionManager::DefinitionManager(InputManager* parent)
    : QWidget(parent)
{
	InitializeDefinitionManager();
}

auto DefinitionManager::InitializeDefinitionManager() -> void
{
    auto* layout = new QGridLayout(this);
    layout->setSpacing(0);
    auto* muInput = new QDoubleSpinBox(this);
    muInput->setRange(-10.0, 10.0);
    muInput->setValue(DefinitionDefault::mu);
    muInput->setSingleStep(0.1);
    m_inputs[ModifiedDefinitionParam::MU] = muInput;
    layout->addWidget(new QLabel("Drift (μ):"));
    layout->addWidget(muInput);

    auto* sigmaInput = new QDoubleSpinBox(this);
    sigmaInput->setRange(0.0, 10.0);
    sigmaInput->setValue(DefinitionDefault::sigma);
    sigmaInput->setSingleStep(0.1);
    m_inputs[ModifiedDefinitionParam::SIGMA] = sigmaInput;
    layout->addWidget(new QLabel("Volatility (σ):"));
    layout->addWidget(sigmaInput);

    auto* startValueInput = new QDoubleSpinBox(this);
    startValueInput->setRange(-20, 20.0);
    startValueInput->setValue(DefinitionDefault::startValue);
    startValueInput->setSingleStep(1);
    m_inputs[ModifiedDefinitionParam::STARTVALUE] = startValueInput;
    layout->addWidget(new QLabel("Start Value:"));
    layout->addWidget(startValueInput);
    setLayout(layout);

    connect(
        muInput,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        [this]() { OnProcessDefinitionModified(ModifiedDefinitionParam::MU); });
    connect(
        sigmaInput,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        [this]() { OnProcessDefinitionModified(ModifiedDefinitionParam::SIGMA); });
    connect(
        startValueInput,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        [this]() { OnProcessDefinitionModified(ModifiedDefinitionParam::STARTVALUE); });
}

auto DefinitionManager::GetMuValue() const -> double
{
    return m_inputs.at(ModifiedDefinitionParam::MU)->value();
}

auto DefinitionManager::GetSigmaValue() const -> double
{
    return m_inputs.at(ModifiedDefinitionParam::SIGMA)->value();
}

auto DefinitionManager::GetStartValue() const -> double
{
    return m_inputs.at(ModifiedDefinitionParam::STARTVALUE)->value();
}

auto DefinitionManager::Parent() const -> InputManager*
{
    return qobject_cast<InputManager*>(parent());
}

auto DefinitionManager::OnProcessDefinitionModified(const ModifiedDefinitionParam param) const -> void
{
    Parent()->OnProcessDefinitionModified(param, m_inputs.at(param)->value());
}