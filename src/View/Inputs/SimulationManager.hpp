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
	//template<IntOrDouble T>
	//auto SimulationModifiedCb(SimulationWidget param) {
	//	return [this, param](T newValue) {
	//		Parent()->OnSimulationParametersModified(param, newValue);
	//		};
	//};
private:
	std::unordered_map<SimulationWidget, QWidget*> m_widgets;
};