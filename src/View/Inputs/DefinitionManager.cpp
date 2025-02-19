#include "DefinitionManager.hpp"
#include "DefaultConstants.hpp"
#include "InputManager.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/qdoublespinbox>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlabel.h>

DefinitionManager::DefinitionManager(InputManager *parent) : QGroupBox(parent)
{
    AddSpinboxes();
    InitializeDesign();
}


auto DefinitionManager::AddSpinboxes() -> void
{
    auto* layout = new QFormLayout(this);

    // Create spinboxes with compact style
    auto createSpinBox = [this](double min, double max, double default_value, double step) {
        auto* spinBox = new QDoubleSpinBox(this);
        spinBox->setRange(min, max);
        spinBox->setValue(default_value);
        spinBox->setSingleStep(step);
        return spinBox;
        };

    auto* muInput = createSpinBox(-10.0, 10.0, DefinitionDefault::mu, 0.1);
    m_inputs[ModifiedDefinitionParam::MU] = muInput;
    layout->addRow(new QLabel("Drift (μ):", this), muInput);

    auto* sigmaInput = createSpinBox(0.0, 10.0, DefinitionDefault::sigma, 0.1);
    m_inputs[ModifiedDefinitionParam::SIGMA] = sigmaInput;
    layout->addRow(new QLabel("Diffusion (σ):", this), sigmaInput);

    auto* startValueInput = createSpinBox(-20.0, 20.0, DefinitionDefault::startValue, 1.0);
    m_inputs[ModifiedDefinitionParam::STARTVALUE] = startValueInput;
    layout->addRow(new QLabel("Start (X<sub>0</sub>):", this), startValueInput);

    setLayout(layout);

    // Connect signals
    connect(muInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
        [this]() { OnProcessDefinitionModified(ModifiedDefinitionParam::MU); });
    connect(sigmaInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
        [this]() { OnProcessDefinitionModified(ModifiedDefinitionParam::SIGMA); });
    connect(startValueInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
        [this]() { OnProcessDefinitionModified(ModifiedDefinitionParam::STARTVALUE); });
}

auto DefinitionManager::InitializeDesign() -> void
{
    setTitle("Definition");
    setStyleSheet(GUI::GroupBoxDescription() + GUI::SpinBoxDescription());
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

auto DefinitionManager::Parent() const -> InputManager *
{
    return qobject_cast<InputManager *>(parent());
}

auto DefinitionManager::OnProcessDefinitionModified(const ModifiedDefinitionParam param) const -> void
{
    Parent()->OnProcessDefinitionModified(param, m_inputs.at(param)->value());
}
