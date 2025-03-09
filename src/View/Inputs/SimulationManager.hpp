#pragma once
#include "Types.hpp"
#include "InputManager.hpp"

class SimulationManager : public InputManagerGroupBox
{
	Q_OBJECT
public:
	explicit SimulationManager(InputManager* parent);
private:
	auto AddComboBoxes() -> void;
	auto AddSpinBoxes() -> void;
	auto InitializeDesign() -> void;
private:
	template <UInt64OrDouble T>
	auto SimulationModifiedCb(SimulationWidget param) const;
	std::unordered_map<SimulationWidget, QWidget*> m_widgets;
};