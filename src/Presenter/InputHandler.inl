#pragma once
#include "Types.hpp"
#include <cassert>
#include <type_traits>
#include <cstdint>

template <IntOrDouble T>
auto InputHandler::OnSimulationParametersModified(const SimulationWidget param, T userValue) -> void
{
	switch (param) {
	case SimulationWidget::TIME:
		if constexpr (std::is_same_v<T, int>)
			m_simulationParameters->time = userValue;
		else
			assert(false && "'Time' parameter must be used with int type");
		break;
	case SimulationWidget::dt:
		m_simulationParameters->dt = userValue;
		break;
	case SimulationWidget::SAMPLES:
		if constexpr (std::is_same_v<T, int>)
			m_simulationParameters->samples = userValue;
		else
			assert(false && "'Samples' parameter must be used with int type");
		break;
	default:
		assert(false);
	}
}