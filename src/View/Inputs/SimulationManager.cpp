#include "SimulationManager.hpp"
#include "InputManager.hpp"

SimulationManager::SimulationManager(InputManager* parent)
	: QGroupBox(parent)
{
    //AddDefinitionWidgets();
}


//auto SimulationManager::AddDefinitionWidgets() -> void
//{
//    auto* layout = new QFormLayout(this);
//
//    auto* muInput = GUI::CreateSpinBox(this, 0, 1000, DefinitionDefault::mu, 0.05);
//    m_widgets[ModifiedDefinitionParam::MU] = muInput;
//    layout->addRow(new QLabel("Time:", this), muInput);
//
//    auto* sigmaInput = GUI::CreateSpinBox(this, 0.0, 20, DefinitionDefault::sigma, 0.1);
//    m_widgets[ModifiedDefinitionParam::SIGMA] = sigmaInput;
//    layout->addRow(new QLabel("Diffusion (σ):", this), sigmaInput);
//
//    auto* startValueInput = GUI::CreateSpinBox(this, -20.0, 20.0, DefinitionDefault::startValue, 1.0);
//    m_widgets[ModifiedDefinitionParam::STARTVALUE] = startValueInput;
//    layout->addRow(new QLabel("Start (X<sub>0</sub>):", this), startValueInput);
//
//    setLayout(layout);
//
//    connect(muInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
//        [this]() { OnProcessDefinitionModified(ModifiedDefinitionParam::MU); });
//    connect(sigmaInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
//        [this]() { OnProcessDefinitionModified(ModifiedDefinitionParam::SIGMA); });
//    connect(startValueInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
//        [this]() { OnProcessDefinitionModified(ModifiedDefinitionParam::STARTVALUE); });
//}