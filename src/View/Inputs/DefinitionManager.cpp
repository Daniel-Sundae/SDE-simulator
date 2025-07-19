#include "DefinitionManager.hpp"
#include "Constants.hpp"
#include "InputDispatcher.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>

DefinitionManager::DefinitionManager(InputDispatcher *parent)
    : InputDispatcherGroupBox(parent)
{
    addDefinitionWidgets();
    initializeDesign();
}

void DefinitionManager::updateWidgetProperties(ProcessType process, bool initialize){
    auto* muWidget = qobject_cast<QDoubleSpinBox*>(m_widgets[DefinitionWidget::MU]);
    ProcessData::Constants muData = ProcessData::getMuData(process);
    muWidget->setRange(muData.allowedValues.first, muData.allowedValues.second);
    muWidget->setSingleStep(muData.incrementSize);

    auto* sigmaWidget = qobject_cast<QDoubleSpinBox*>(m_widgets[DefinitionWidget::SIGMA]);
    ProcessData::Constants sigmaData = ProcessData::getSigmaData(process);
    sigmaWidget->setRange(sigmaData.allowedValues.first, sigmaData.allowedValues.second);
    sigmaWidget->setSingleStep(sigmaData.incrementSize);

    auto* startValueWidget = qobject_cast<QDoubleSpinBox*>(m_widgets[DefinitionWidget::STARTVALUE]);
    ProcessData::Constants startValueData = ProcessData::getStartValueData(process);
    startValueWidget->setRange(startValueData.allowedValues.first, startValueData.allowedValues.second);
    startValueWidget->setSingleStep(startValueData.incrementSize);

    if (initialize){
        muWidget->setValue(muData.defaultValue);
        sigmaWidget->setValue(sigmaData.defaultValue);
        startValueWidget->setValue(startValueData.defaultValue);
    }
}

void DefinitionManager::addDefinitionWidgets(){
    std::array<ProcessType, 3> processTypes = {
        ProcessType::BM,
        ProcessType::GBM,
        ProcessType::OU,
    };
    auto* processes = new QComboBox(this);

    processes->insertItem(0, QString::fromStdString(std::string(ProcessData::getAcronym<ProcessType::BM>())));
    processes->setItemData(0, QString::fromStdString(std::string(ProcessData::getName<ProcessType::BM>())), Qt::ToolTipRole);
    processes->insertItem(1, QString::fromStdString(std::string(ProcessData::getAcronym<ProcessType::GBM>())));
    processes->setItemData(1, QString::fromStdString(std::string(ProcessData::getName<ProcessType::GBM>())), Qt::ToolTipRole);
    processes->insertItem(2, QString::fromStdString(std::string(ProcessData::getAcronym<ProcessType::OU>())));
    processes->setItemData(2, QString::fromStdString(std::string(ProcessData::getName<ProcessType::OU>())), Qt::ToolTipRole);

    processes->setCurrentIndex(0);
    m_widgets[DefinitionWidget::PROCESS] = processes;
    m_widgets[DefinitionWidget::MU] = new QDoubleSpinBox(this);
    m_widgets[DefinitionWidget::SIGMA] = new QDoubleSpinBox(this);
    m_widgets[DefinitionWidget::STARTVALUE] = new QDoubleSpinBox(this);
    updateWidgetProperties(DefaultConstants::process, true);
    auto definitionModifiedCb = [this](DefinitionWidget param) {
        return [this, param](double newValue) {
            Parent()->onProcessDefinitionModified(param, newValue);
            };
        };

    connect(
        qobject_cast<QComboBox*>(m_widgets[DefinitionWidget::PROCESS]),
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this, processTypes](int newIndex) {
            Parent()->onProcessTypeModified(processTypes[newIndex]);
            updateWidgetProperties(processTypes[newIndex]);
        }
    );
    connect(
        qobject_cast<QDoubleSpinBox*>(m_widgets[DefinitionWidget::MU]),
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        definitionModifiedCb(DefinitionWidget::MU)
    );
    connect(
        qobject_cast<QDoubleSpinBox*>(m_widgets[DefinitionWidget::SIGMA]),
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        definitionModifiedCb(DefinitionWidget::SIGMA)
    );
    connect(
        qobject_cast<QDoubleSpinBox*>(m_widgets[DefinitionWidget::STARTVALUE]),
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        definitionModifiedCb(DefinitionWidget::STARTVALUE)
    );

}

void DefinitionManager::initializeDesign(){
    setTitle("Definition");
    setStyleSheet(GUI::groupBoxDescription() + GUI::comboBoxDescription() + GUI::spinBoxDescription());
    auto* definitionLayout = new QFormLayout(this);
    definitionLayout->addRow(new QLabel("Process:", this), m_widgets[DefinitionWidget::PROCESS]);
    definitionLayout->addRow(new QLabel("Drift (μ):", this), m_widgets[DefinitionWidget::MU]);
    definitionLayout->addRow(new QLabel("Diffusion (σ):", this), m_widgets[DefinitionWidget::SIGMA]);
    definitionLayout->addRow(new QLabel("Start (X<sub>0</sub>):", this), m_widgets[DefinitionWidget::STARTVALUE]);
}
