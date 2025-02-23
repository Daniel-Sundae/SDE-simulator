#pragma once
#include "Types.hpp"
#include <QtWidgets/qgroupbox.h>
class InputManager;

class SimulationManager : public QGroupBox
{
	Q_OBJECT
public:
	explicit SimulationManager(InputManager* parent);
private:
	auto AddComboBoxes() -> void;
	auto AddSpinBoxes() -> void;
	auto InitializeDesign() -> void;
	auto Parent() const -> InputManager*;
private:
	std::unordered_map<SimulationWidget, QWidget*> m_widgets;
};