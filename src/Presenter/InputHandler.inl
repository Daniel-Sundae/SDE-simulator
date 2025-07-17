#pragma once
#include "Types.hpp"
#include "Utils.hpp"
#include <cassert>
#include <type_traits>
#include <cstdint>

template <IntOrDouble T>
void InputHandler::onSimulationParameterModified(const SimulationWidget param, T userValue){
	switch (param) {
		case SimulationWidget::TIME:
			if constexpr (std::is_same_v<T, int>)
				m_simulationParameters->time = userValue;
			else
				assert(false && "'Time' parameter must be invoked with int type");
			break;
		case SimulationWidget::dt:
			m_simulationParameters->dt = userValue;
			break;
		case SimulationWidget::SAMPLES:
			if constexpr (std::is_same_v<T, int>)
				m_simulationParameters->samples = userValue;
			else
				assert(false && "'Samples' parameter must be invoked with int type");
			break;
		default:
			assert(false);
	}
}

template <IntOrBool T>
void InputHandler::onSettingsParameterModified(const SettingsWidget param, T userValue){
	switch (param){
		case SettingsWidget::THREADS:
			if constexpr (std::is_same_v<T, bool>)
				m_settingsParameters->useThreading = userValue;
			else
				assert(false && "Threading checkbox parameter must be invoked with bool type");
			break;
		case SettingsWidget::FIXSEED:
			if constexpr (std::is_same_v<T, int>)
				if(userValue == 0){
					m_settingsParameters->useSeed = {false, 0};
				}
				else{
					m_settingsParameters->useSeed = {true, userValue};
				}
			else
				assert(false && "Seeding checkbox parameter must be invoked with int type");
			break;
		default:
			assert(false);
	}			
}