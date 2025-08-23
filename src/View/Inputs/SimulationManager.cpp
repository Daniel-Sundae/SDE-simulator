#include "SimulationManager.hpp"
#include "Constants.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>


SimulationManager::SimulationManager(InputDispatcher* parent)
    : InputDispatcherGroupBox(parent)
{
    addComboBoxes();
    addSpinBoxes();
    initializeDesign();
}

void SimulationManager::addComboBoxes(){
    std::array<std::pair< SolverType, std::pair<std::string, std::string> >, 3> solverTypes = {
    std::pair{SolverType::EULER_MARUYAMA, std::pair{"EM", "Euler-Maruyama"}},
    std::pair{SolverType::RUNGE_KUTTA, std::pair{"RK", "Runge-Kutta"}},
    std::pair{SolverType::MILSTEIN, std::pair{"MILSTEIN", "Milstein"}},
    };
    auto* solvers = new QComboBox(this);
    for (size_t i = 0; i < solverTypes.size(); ++i) {
        solvers->insertItem(static_cast<int>(i), QString::fromStdString(solverTypes[i].second.first));
        solvers->setItemData(static_cast<int>(i), QString::fromStdString(solverTypes[i].second.second), Qt::ToolTipRole);
    }

    m_widgets[SimulationWidget::SOLVER] = solvers;
    connect(
        qobject_cast<QComboBox*>(m_widgets[SimulationWidget::SOLVER]),
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this, solverTypes]() {
            int currentIdx = qobject_cast<QComboBox*>(m_widgets[SimulationWidget::SOLVER])->currentIndex();
            SolverType newSolver = solverTypes[static_cast<size_t>(currentIdx)].first;
            Parent()->onSolverTypeModified(newSolver);
        }
    );
}

template <IntOrDouble T>
auto SimulationManager::simulationModifiedCb(SimulationWidget param) const{
    return [this, param](T newValue) {
        Parent()->onSimulationParameterModified(param, newValue);
    };
}

void SimulationManager::addSpinBoxes(){
    auto* timeWidget = new QSpinBox(this);
    timeWidget->setValue(static_cast<uint64_t>(DefaultConstants::Simulation::time));
    timeWidget->setRange(1, 50);
    timeWidget->setSingleStep(5);
    m_widgets[SimulationWidget::TIME] = timeWidget;
    auto* dtWidget = new QDoubleSpinBox(this);
    dtWidget->setValue(DefaultConstants::Simulation::dt);
    dtWidget->setDecimals(3);
    dtWidget->setRange(0.0, 0.5); 
    dtWidget->setSingleStep(0.001);
    m_widgets[SimulationWidget::dt] = dtWidget;
    auto* samplesWidget = new QSpinBox(this);
    samplesWidget->setValue(1);
    samplesWidget->setRange(1, 1000000);
    samplesWidget->setSingleStep(10);
    m_widgets[SimulationWidget::SAMPLES] = samplesWidget;

    connect(
        timeWidget,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        simulationModifiedCb<int>(SimulationWidget::TIME)
    );

    connect(
        dtWidget,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        simulationModifiedCb<double>(SimulationWidget::dt)
    );

    connect(
        samplesWidget,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        simulationModifiedCb<int>(SimulationWidget::SAMPLES)
    );
}

void SimulationManager::initializeDesign(){
    setTitle("Simulation");
    setStyleSheet(GUI::groupBoxDescription() + GUI::comboBoxDescription() + GUI::spinBoxDescription());
    auto* simulationLayout = new QFormLayout(this);
    simulationLayout->addRow(new QLabel("Solver:", this), m_widgets[SimulationWidget::SOLVER]);
    simulationLayout->addRow(new QLabel("Time (T):", this), m_widgets[SimulationWidget::TIME]);
    simulationLayout->addRow(new QLabel("Time step (dt):", this), m_widgets[SimulationWidget::dt]);
    auto* samplesLabel = new QLabel("Samples:", this);
    samplesLabel->setToolTip("Nr samples (paths) to generate.");
    simulationLayout->addRow(samplesLabel, m_widgets[SimulationWidget::SAMPLES]);

}
