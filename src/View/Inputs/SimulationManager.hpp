#pragma once
#include "Types.hpp"
#include "InputDispatcher.hpp"

class SimulationManager : public InputDispatcherGroupBox
{
	Q_OBJECT
public:
	explicit SimulationManager(InputDispatcher* parent);
private:
	auto AddComboBoxes() -> void;
	auto AddSpinBoxes() -> void;
	auto InitializeDesign() -> void;
private:
	template <IntOrDouble T>
	auto SimulationModifiedCb(SimulationWidget param) const;
	std::unordered_map<SimulationWidget, QWidget*> m_widgets;
};