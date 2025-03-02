#include "SimulationManager.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlabel.h>


SimulationManager::SimulationManager(InputManager* parent)
	: InputManagerGroupBox(parent)
{
    AddComboBoxes();
    AddSpinBoxes();
    InitializeDesign();
}

auto SimulationManager::AddComboBoxes() -> void
{
    std::array<std::pair< SolverType, std::pair<std::string, std::string> >, 2> solverTypes = {
    std::pair{SolverType::EULER_MARUYAMA, std::pair{"EM", "Euler-Maruyama"}},
    std::pair{SolverType::RUNGE_KUTTA, std::pair{"RK", "Runge-Kutta"}},
    };
    auto* solvers = new QComboBox(this);
    for (int i = 0; i < solverTypes.size(); ++i) {
        solvers->insertItem(i, QString::fromStdString(solverTypes[i].second.first));
        solvers->setItemData(i, QString::fromStdString(solverTypes[i].second.second), Qt::ToolTipRole);
    }

    m_widgets[SimulationWidget::SOLVER] = solvers;
    connect(
        qobject_cast<QComboBox*>(m_widgets[SimulationWidget::SOLVER]),
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this, solverTypes]() {
            SolverType newSolver = solverTypes[qobject_cast<QComboBox*>(m_widgets[SimulationWidget::SOLVER])->currentIndex()].first;
            Parent()->OnSolverTypeModified(newSolver);
        }
    );
}

auto SimulationManager::AddSpinBoxes() -> void
{
    auto* timeWidget = new QDoubleSpinBox(this);
    timeWidget->setValue(100);
    timeWidget->setMinimum(0);
    timeWidget->setMaximum(1000);
    timeWidget->setSingleStep(50);
    m_widgets[SimulationWidget::TIME] = timeWidget;
    auto* dtWidget = new QDoubleSpinBox(this);
    dtWidget->setValue(0.1);
    dtWidget->setMinimum(0);
    dtWidget->setMaximum(1);
    dtWidget->setSingleStep(0.1);
    m_widgets[SimulationWidget::dt] = dtWidget;
    auto* samplesWidget = new QSpinBox(this);
    samplesWidget->setValue(1);
    samplesWidget->setMinimum(1);
    samplesWidget->setMaximum(100);
    samplesWidget->setSingleStep(1);
    m_widgets[SimulationWidget::SAMPLES] = samplesWidget;

    auto simulationModifiedCb = [this](SimulationWidget param) {
	    return [this, param]<typename T> requires IntOrDouble<T>(T newValue) {
		    Parent()->OnSimulationParametersModified(param, newValue);
		};
    };

    connect(
        timeWidget,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        simulationModifiedCb(SimulationWidget::TIME)
    );

    connect(
        dtWidget,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        simulationModifiedCb(SimulationWidget::dt)
    );

    connect(
        samplesWidget,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        simulationModifiedCb(SimulationWidget::SAMPLES)
    );
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