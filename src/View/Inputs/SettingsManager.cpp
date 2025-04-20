#include "SettingsManager.hpp"
#include "InputDispatcher.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>


SettingsManager::SettingsManager(InputDispatcher* parent)
	: InputDispatcherGroupBox(parent)
{
    AddMultiThreadCheckBox();
    AddSeedWidget();
    InitializeDesign();
}

auto SettingsManager::AddMultiThreadCheckBox() -> void
{
    auto* multiThreadingCheckbBox = new QCheckBox(this);
    m_widgets[SettingsWidget::THREADS] = multiThreadingCheckbBox;
    multiThreadingCheckbBox->setChecked(true);
    connect(
        multiThreadingCheckbBox,
        &QCheckBox::toggled,
        this,
        [this](bool checked){Parent()->OnSettingsParameterModified<bool>(SettingsWidget::THREADS, checked);}
    );
}

auto SettingsManager::AddSeedWidget() -> void
{
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
            Parent()->OnSettingsParameterModified<uint32_t>(
                SettingsWidget::FIXSEED,
                toggled ? static_cast<uint32_t>(seedSpinBox->value()) : 0
            );
        }
    );
    auto* seedLayout = new QHBoxLayout(seedWidget);
    seedLayout->addWidget(seedCheckBox);
    seedLayout->addWidget(seedSpinBox);
    seedLayout->setContentsMargins(0, 0, 0, 0);
}

auto SettingsManager::InitializeDesign() -> void
{
    setTitle("Settings");
    setStyleSheet(GUI::GroupBoxDescription() + GUI::CheckBoxDescription());
    auto* simulationLayout = new QFormLayout(this);
    auto* multiThreadingLabel = new QLabel("Multithreading:", this);
    multiThreadingLabel->setToolTip("Toggle multithreading to compare performance.");
    simulationLayout->addRow(multiThreadingLabel, m_widgets[SettingsWidget::THREADS]);
    auto* fixSeedLabel = new QLabel("Fix seed:", this);
    fixSeedLabel->setToolTip("Set source of randomness.");
    simulationLayout->addRow(fixSeedLabel, m_widgets[SettingsWidget::FIXSEED]);
}
