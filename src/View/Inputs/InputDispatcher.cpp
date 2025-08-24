#include "InputDispatcher.hpp"
#include "ActionManager.hpp"
#include "DefinitionManager.hpp"
#include "SimulationManager.hpp"
#include "SettingsManager.hpp"

InputDispatcher::InputDispatcher(QWidget* parent)
    : QWidget(parent)
    , m_actionManager(new ActionManager(this))
    , m_definitionManager(new DefinitionManager(this))
    , m_simulationManager(new SimulationManager(this))
    , m_settingsManager(new SettingsManager(this))
{
    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_actionManager, 1);
    layout->addWidget(m_definitionManager, 2);
    layout->addWidget(m_simulationManager, 2);
    layout->addWidget(m_settingsManager, 1);
}