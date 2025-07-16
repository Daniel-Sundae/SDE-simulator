#pragma once
#include "Types.hpp"
#include "InputDispatcher.hpp"

class SimulationManager : public InputDispatcherGroupBox
{
	Q_OBJECT
public:
	explicit SimulationManager(InputDispatcher* parent);
private:
	void addComboBoxes();
	void addSpinBoxes();
	void initializeDesign();
private:
	template <IntOrDouble T> auto simulationModifiedCb(SimulationWidget param) const;
	std::unordered_map<SimulationWidget, QWidget*> m_widgets;
};