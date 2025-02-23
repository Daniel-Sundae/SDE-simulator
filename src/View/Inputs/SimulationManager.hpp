#pragma once

#include <QtWidgets/qgroupbox.h>
class InputManager;

class SimulationManager : public QGroupBox
{
	Q_OBJECT
public:
	explicit SimulationManager(InputManager* parent);
private:
	//auto AddDefinitionWidgets() -> void;
	//auto InitializeSimulationManager() -> void;


};