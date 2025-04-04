#include "DefinitionManager.hpp"
#include "Constants.hpp"
#include "InputManager.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/qdoublespinbox>
#include <QtWidgets/qcombobox>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlabel.h>

DefinitionManager::DefinitionManager(InputManager *parent)
    : InputManagerGroupBox(parent)
{
    AddDefinitionWidgets();
    InitializeDesign();
}

auto DefinitionManager::UpdateWidgetProperties(ProcessType process, bool initialize) -> void
{
    auto* muWidget = qobject_cast<QDoubleSpinBox*>(m_widgets[DefinitionWidget::MU]);
    ProcessData::Constants muData = ProcessData::GetMuData(process);
    muWidget->setRange(muData.allowedValues.first, muData.allowedValues.second);
    muWidget->setSingleStep(muData.incrementSize);

    auto* sigmaWidget = qobject_cast<QDoubleSpinBox*>(m_widgets[DefinitionWidget::SIGMA]);
    ProcessData::Constants sigmaData = ProcessData::GetSigmaData(process);
    sigmaWidget->setRange(sigmaData.allowedValues.first, sigmaData.allowedValues.second);
    sigmaWidget->setSingleStep(sigmaData.incrementSize);

    auto* startValueWidget = qobject_cast<QDoubleSpinBox*>(m_widgets[DefinitionWidget::STARTVALUE]);
    ProcessData::Constants startValueData = ProcessData::GetStartValueData(process);
    startValueWidget->setRange(startValueData.allowedValues.first, startValueData.allowedValues.second);
    startValueWidget->setSingleStep(startValueData.incrementSize);

    if (initialize){
        muWidget->setValue(muData.defaultValue);
        sigmaWidget->setValue(sigmaData.defaultValue);
        startValueWidget->setValue(startValueData.defaultValue);
    }
}

auto DefinitionManager::AddDefinitionWidgets() -> void
{
    std::array<ProcessType, 2> processTypes = {
        ProcessType::BM,
        ProcessType::GBM,
    };
    auto* processes = new QComboBox(this);
    for (int i = 0; i < static_cast<int>(processTypes.size()); ++i) {
        processes->insertItem(i, QString::fromStdString(std::string(ProcessData::GetAcronym(processTypes[i]))));
        processes->setItemData(i, QString::fromStdString(std::string(ProcessData::GetName(processTypes[i]))), Qt::ToolTipRole);
    }
    processes->setCurrentIndex(0);
    ProcessType defaultProcess = processTypes[0];
    m_widgets[DefinitionWidget::PROCESS] = processes;
    m_widgets[DefinitionWidget::MU] = new QDoubleSpinBox(this);
    m_widgets[DefinitionWidget::SIGMA] = new QDoubleSpinBox(this);
    m_widgets[DefinitionWidget::STARTVALUE] = new QDoubleSpinBox(this);
    UpdateWidgetProperties(defaultProcess, true);
    auto definitionModifiedCb = [this](DefinitionWidget param) {
        return [this, param](double newValue) {
            Parent()->OnProcessDefinitionModified(param, newValue);
            };
        };

    connect(
        qobject_cast<QComboBox*>(m_widgets[DefinitionWidget::PROCESS]),
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this, processTypes](int newIndex) {
            Parent()->OnProcessTypeModified(processTypes[newIndex]);
            UpdateWidgetProperties(processTypes[newIndex]);
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

auto DefinitionManager::InitializeDesign() -> void
{
    setTitle("Definition");
    setStyleSheet(GUI::GroupBoxDescription() + GUI::ComboBoxDescription() + GUI::SpinBoxDescription());
    auto* definitionLayout = new QFormLayout(this);
    definitionLayout->addRow(new QLabel("Process:", this), m_widgets[DefinitionWidget::PROCESS]);
    definitionLayout->addRow(new QLabel("Drift (μ):", this), m_widgets[DefinitionWidget::MU]);
    definitionLayout->addRow(new QLabel("Diffusion (σ):", this), m_widgets[DefinitionWidget::SIGMA]);
    definitionLayout->addRow(new QLabel("Start (X<sub>0</sub>):", this), m_widgets[DefinitionWidget::STARTVALUE]);
}

