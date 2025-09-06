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
    , m_process(new QComboBox(this))
    , m_muWidget(new QDoubleSpinBox(this))
    , m_sigmaWidget(new QDoubleSpinBox(this))
    , m_X0Widget(new QDoubleSpinBox(this))
{
    addDefinitionWidgets();
    initializeDesign();
}

void DefinitionManager::updateWidgetProperties(ProcessType process){
    Constants muData = getField(FieldTags::muData{}, process);
    m_muWidget->setRange(muData.allowedValues.first, muData.allowedValues.second);
    m_muWidget->setValue(muData.defaultValue);
    m_muWidget->setSingleStep(muData.incrementSize);

    Constants sigmaData = getField(FieldTags::sigmaData{}, process);
    m_sigmaWidget->setRange(sigmaData.allowedValues.first, sigmaData.allowedValues.second);
    m_sigmaWidget->setValue(sigmaData.defaultValue);
    m_sigmaWidget->setSingleStep(sigmaData.incrementSize);

    Constants X0 = getField(FieldTags::X0{}, process);
    m_X0Widget->setRange(X0.allowedValues.first, X0.allowedValues.second);
    m_X0Widget->setValue(X0.defaultValue);
    m_X0Widget->setSingleStep(X0.incrementSize);
}

void DefinitionManager::addDefinitionWidgets(){
    std::array<ProcessType, 3> processTypes = {
        ProcessType::BM,
        ProcessType::GBM,
        ProcessType::OU,
    };

    for (size_t i = 0; i < processTypes.size(); ++i) {
        m_process->insertItem(static_cast<int>(i), QString::fromStdString(std::string( getField(FieldTags::acronym{}, processTypes[i]) )));
        m_process->setItemData(static_cast<int>(i), QString::fromStdString(std::string( getField(FieldTags::name{}, processTypes[i]) )), Qt::ToolTipRole);
    }

    m_process->setCurrentIndex(0);
    updateWidgetProperties(DefaultConstants::process);
    auto definitionModifiedCb = [this](DefinitionWidget param) {
        return [this, param](double newValue) {
            Parent()->onDefinitionParametersModified(param, newValue);
            };
        };

    connect(
        m_process,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this, processTypes](int newIndex) {
            Parent()->onProcessTypeModified(processTypes[newIndex]);
            updateWidgetProperties(processTypes[newIndex]);
        }
    );
    connect(
        m_muWidget,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        definitionModifiedCb(DefinitionWidget::MU)
    );
    connect(
        m_sigmaWidget,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        definitionModifiedCb(DefinitionWidget::SIGMA)
    );
    connect(
        m_X0Widget,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        definitionModifiedCb(DefinitionWidget::X0)
    );
}

void DefinitionManager::initializeDesign(){
    setTitle("Definition");
    setStyleSheet(GUI::groupBoxDescription() + GUI::comboBoxDescription() + GUI::spinBoxDescription());
    auto* definitionLayout = new QFormLayout(this);
    definitionLayout->addRow(new QLabel("Process:", this), m_process);
    definitionLayout->addRow(new QLabel("Drift (μ):", this), m_muWidget);
    definitionLayout->addRow(new QLabel("Diffusion (σ):", this), m_sigmaWidget);
    definitionLayout->addRow(new QLabel("Start (X<sub>0</sub>):", this), m_X0Widget);
}
