#include "SettingsManager.hpp"
#include "InputDispatcher.hpp"
#include "ViewUtils.hpp"
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>


SettingsManager::SettingsManager(InputDispatcher* parent)
    : InputDispatcherGroupBox(parent)
    , m_multiThreadingCheckBox(new QCheckBox(this))
    , m_seedWidget(new QWidget(this))
{
    addMultiThreadCheckBox();
    addSeedWidget();
    initializeDesign();
}

void SettingsManager::addMultiThreadCheckBox(){
    m_multiThreadingCheckBox->setChecked(true);
    connect(
        m_multiThreadingCheckBox,
        &QCheckBox::toggled,
        this,
        [this](bool checked){Parent()->onSettingsParameterModified(SettingsWidget::THREADS, checked);}
    );
}

void SettingsManager::addSeedWidget(){
    m_seedWidget = new QWidget(this);
    auto* seedCheckBox = new QCheckBox(m_seedWidget);
    seedCheckBox->setChecked(false);
    auto* seedSpinBox = new QSpinBox(m_seedWidget);
    seedSpinBox->setValue(1);
    seedSpinBox->setMinimum(0);
    seedSpinBox->setMaximum(10000);
    seedSpinBox->setSingleStep(1);
    seedSpinBox->setEnabled(false);

    connect(
        seedCheckBox, &QCheckBox::toggled,
        this, [this, seedSpinBox](bool toggled) {
            seedSpinBox->setEnabled(toggled);
            Parent()->onSettingsParameterModified(
                SettingsWidget::FIXSEED,
                toggled ? static_cast<int>(seedSpinBox->value()) : 0
            );
        }
    );

    connect(
        seedSpinBox, &QSpinBox::valueChanged,
        this, [this](int newValue) {
            Parent()->onSettingsParameterModified(
                SettingsWidget::FIXSEED,
                newValue
            );
        }
    );

    auto* seedLayout = new QHBoxLayout(m_seedWidget);
    seedLayout->addWidget(seedCheckBox);
    seedLayout->addWidget(seedSpinBox);
    seedLayout->setContentsMargins(0, 0, 0, 0);
}

void SettingsManager::initializeDesign(){
    setTitle("Settings");
    setStyleSheet(GUI::groupBoxDescription() + GUI::checkBoxDescription());

    auto* fixSeedLabel = new QLabel("Fix seed:", this);
    fixSeedLabel->setToolTip("Set source of randomness.");

    auto* grid = new QGridLayout(this);
    GUI::initGridLayout(grid);

    auto* multiThreadingLabel = new QLabel("Multithreading:", this);
    multiThreadingLabel->setToolTip("Toggle multithreading to compare performance.");
    grid->addWidget(multiThreadingLabel,      0, 0);
    grid->addWidget(m_multiThreadingCheckBox, 0, 1);
    grid->addWidget(fixSeedLabel,             1, 0);
    grid->addWidget(m_seedWidget,             1, 1);
}
