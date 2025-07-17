#include "SettingsManager.hpp"
#include "InputDispatcher.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>


SettingsManager::SettingsManager(InputDispatcher* parent)
    : InputDispatcherGroupBox(parent)
{
    addMultiThreadCheckBox();
    addSeedWidget();
    initializeDesign();
}

void SettingsManager::addMultiThreadCheckBox(){
    auto* multiThreadingCheckbBox = new QCheckBox(this);
    m_widgets[SettingsWidget::THREADS] = multiThreadingCheckbBox;
    multiThreadingCheckbBox->setChecked(true);
    connect(
        multiThreadingCheckbBox,
        &QCheckBox::toggled,
        this,
        [this](bool checked){Parent()->onSettingsParameterModified(SettingsWidget::THREADS, checked);}
    );
}

void SettingsManager::addSeedWidget(){
    auto* seedWidget = new QWidget(this);
    m_widgets[SettingsWidget::FIXSEED] = seedWidget;
    auto* seedCheckBox = new QCheckBox(seedWidget);
    seedCheckBox->setChecked(false);
    auto* seedSpinBox = new QSpinBox(seedWidget);
    seedSpinBox->setValue(1);
    seedSpinBox->setMinimum(1);
    seedSpinBox->setMaximum(10000);
    seedSpinBox->setSingleStep(1);
    seedSpinBox->setEnabled(false);

    connect(
        seedCheckBox,
        &QCheckBox::toggled,
        this,
        [this, seedSpinBox](bool toggled) {
            seedSpinBox->setEnabled(toggled);
            Parent()->onSettingsParameterModified(
                SettingsWidget::FIXSEED,
                toggled ? static_cast<int>(seedSpinBox->value()) : 0
            );
        }
    );

    connect(
        seedSpinBox,
        &QSpinBox::valueChanged,
        this,
        [this](int newValue) {
            Parent()->onSettingsParameterModified(
                SettingsWidget::FIXSEED,
                newValue
            );
        }
    );

    auto* seedLayout = new QHBoxLayout(seedWidget);
    seedLayout->addWidget(seedCheckBox);
    seedLayout->addWidget(seedSpinBox);
    seedLayout->setContentsMargins(0, 0, 0, 0);
}

void SettingsManager::initializeDesign(){
    setTitle("Settings");
    setStyleSheet(GUI::groupBoxDescription() + GUI::checkBoxDescription());
    auto* simulationLayout = new QFormLayout(this);
    auto* multiThreadingLabel = new QLabel("Multithreading:", this);
    multiThreadingLabel->setToolTip("Toggle multithreading to compare performance.");
    simulationLayout->addRow(multiThreadingLabel, m_widgets[SettingsWidget::THREADS]);
    auto* fixSeedLabel = new QLabel("Fix seed:", this);
    fixSeedLabel->setToolTip("Set source of randomness.");
    simulationLayout->addRow(fixSeedLabel, m_widgets[SettingsWidget::FIXSEED]);
}
