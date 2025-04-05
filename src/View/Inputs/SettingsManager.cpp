#include "SettingsManager.hpp"
#include "InputDispatcher.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlabel.h>


SettingsManager::SettingsManager(InputDispatcher* parent)
	: InputDispatcherGroupBox(parent)
{
    AddCheckBoxes();
    AddSeedWidget();
    InitializeDesign();
}

auto SettingsManager::AddCheckBoxes() -> void
{
    auto* multiThreading = new QCheckBox(this);
    multiThreading->setChecked(true);
    m_widgets[SettingsWidget::THREADS] = multiThreading;
    auto* autoUpdate = new QCheckBox(this);
    autoUpdate->setChecked(true);
    m_widgets[SettingsWidget::AUTOUPDATE] = autoUpdate;
}

auto SettingsManager::AddSeedWidget() -> void
{
    auto* seedWidget = new QWidget(this);
    auto* seedCheckBox = new QCheckBox(seedWidget);
    seedCheckBox->setChecked(false);
    auto* seedSpinBox = new QSpinBox(this);
    seedSpinBox->setValue(1);
    seedSpinBox->setMinimum(1);
    seedSpinBox->setMaximum(1000000);
    seedSpinBox->setSingleStep(1);
    seedSpinBox->setEnabled(false);
    connect(seedCheckBox,
        &QCheckBox::toggled,
        seedSpinBox,
        &QSpinBox::setEnabled);

    auto* seedLayout = new QHBoxLayout(seedWidget);
    seedLayout->addWidget(seedCheckBox);
    seedLayout->addWidget(seedSpinBox);
    seedLayout->setContentsMargins(0, 0, 0, 0);
    m_widgets[SettingsWidget::FIXSEED] = seedWidget;
}

auto SettingsManager::InitializeDesign() -> void
{
    setTitle("Settings");
    setStyleSheet(GUI::GroupBoxDescription() + GUI::CheckBoxDescription());
    auto* simulationLayout = new QFormLayout(this);
    auto* multiThreadingLabel = new QLabel("Multithreading:", this);
    multiThreadingLabel->setToolTip("Toggle multithreading to compare performance.");
    simulationLayout->addRow(multiThreadingLabel, m_widgets[SettingsWidget::THREADS]);
    auto* autoUpdateLabel = new QLabel("Auto-update:", this);
    autoUpdateLabel->setToolTip("Resample automatically when Definition changes.");
    simulationLayout->addRow(autoUpdateLabel, m_widgets[SettingsWidget::AUTOUPDATE]);
    auto* fixSeedLabel = new QLabel("Fix seed:", this);
    fixSeedLabel->setToolTip("Set source of randomness.");
    simulationLayout->addRow(fixSeedLabel, m_widgets[SettingsWidget::FIXSEED]);
}