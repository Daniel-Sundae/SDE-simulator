#include "SimulationManager.hpp"
#include "InputManager.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlabel.h>


SimulationManager::SimulationManager(InputManager* parent)
	: QGroupBox(parent)
{
    AddComboBoxes();
    AddSpinBoxes();
    InitializeDesign();
}

auto SimulationManager::AddComboBoxes() -> void
{
    std::array<std::pair< SolverType, std::pair<std::string, std::string> >, 2> solversTypes = {
    std::pair{SolverType::EULER_MARUYAMA, std::pair{"EM", "Euler-Maruyama"}},
    std::pair{SolverType::RUNGE_KUTTA, std::pair{"RK", "Runge-Kutta"}},
    };
    auto* solvers = new QComboBox(this);
    for (int i = 0; i < solversTypes.size(); ++i) {
        solvers->insertItem(i, QString::fromStdString(solversTypes[i].second.first));
        solvers->setItemData(i, QString::fromStdString(solversTypes[i].second.second), Qt::ToolTipRole);
    }

    m_widgets[SimulationWidget::SOLVER] = solvers;
}

auto SimulationManager::AddSpinBoxes() -> void
{
    m_widgets[SimulationWidget::TIME] = GUI::CreateIntSpinBox(this, 0, 10000, SimulationDefault::time, 500);
    m_widgets[SimulationWidget::dt] = GUI::CreateDoubleSpinBox(this, 0, 10, SimulationDefault::dt, 0.1);
    m_widgets[SimulationWidget::SAMPLES] = GUI::CreateIntSpinBox(this, 1, 100, SimulationDefault::samples, 1);
}

auto SimulationManager::InitializeDesign() -> void
{
    setTitle("Simulation");
    setStyleSheet(GUI::GroupBoxDescription() + GUI::ComboBoxDescription() + GUI::SpinBoxDescription());
    auto* simulationLayout = new QFormLayout(this);
    simulationLayout->addRow(new QLabel("Solver:", this), m_widgets[SimulationWidget::SOLVER]);
    simulationLayout->addRow(new QLabel("Time (T):", this), m_widgets[SimulationWidget::TIME]);
    simulationLayout->addRow(new QLabel("Time step (dt):", this), m_widgets[SimulationWidget::dt]);
    auto* samplesLabel = new QLabel("Samples:", this);
    samplesLabel->setToolTip("Nr samples (paths) to generate.");
    simulationLayout->addRow(samplesLabel, m_widgets[SimulationWidget::SAMPLES]);

}

auto SimulationManager::Parent() const -> InputManager*
{
    return qobject_cast<InputManager*>(parent());
}

//auto SimulationManager::OnProcessTypeModified(const ProcessType newType) const -> void
//{
//    Parent()->OnProcessTypeModified(newType);
//}
//
//auto SimulationManager::OnProcessSimulationModified(const SimulationWidget param) const -> void
//{
//    Parent()->OnProcessSimulationModified(param, qobject_cast<QDoubleSpinBox*>(m_widgets.at(param))->value());
//}
