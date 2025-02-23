#include "SettingsManager.hpp"
#include "InputManager.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlabel.h>


SettingsManager::SettingsManager(InputManager* parent)
	: QGroupBox(parent)
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
    auto* autoClear = new QCheckBox(this);
    autoClear->setChecked(true);
    m_widgets[SettingsWidget::AUTOCLEAR] = autoClear;
}

auto SettingsManager::AddSeedWidget() -> void
{
    auto* seedWidget = new QWidget(this);
    auto* seedCheckBox = new QCheckBox(seedWidget);
    seedCheckBox->setChecked(false);
    auto* seedSpinBox = GUI::CreateIntSpinBox(seedWidget, 1, 1000000, 1, 1);
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
    auto* autoClearLabel = new QLabel("Auto-clear:", this);
    autoClearLabel->setToolTip("Clear existing samples automatically.");
    simulationLayout->addRow(autoClearLabel, m_widgets[SettingsWidget::AUTOCLEAR]);
    auto* fixSeedLabel = new QLabel("Fix seed:", this);
    fixSeedLabel->setToolTip("Set source of randomness.");
    simulationLayout->addRow(fixSeedLabel, m_widgets[SettingsWidget::FIXSEED]);
}

auto SettingsManager::Parent() const -> InputManager*
{
    return qobject_cast<InputManager*>(parent());
}
