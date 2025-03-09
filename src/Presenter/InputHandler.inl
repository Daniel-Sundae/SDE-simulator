#pragma once
#include "Types.hpp"
#include <cassert>
#include <type_traits>

template <UInt64OrDouble T>
auto InputHandler::OnSimulationParametersModified(const SimulationWidget param, T userValue) -> void
{
	switch (param) {
	case SimulationWidget::TIME:
		m_simulationParameters->time = userValue;
		break;
	case SimulationWidget::dt:
		m_simulationParameters->dt = userValue;
		break;
	case SimulationWidget::SAMPLES:
		if constexpr (std::is_same_v<T, int>)
			m_simulationParameters->samples = userValue;
		else
			assert(false && "SAMPLES parameter must be used with int type");
		break;
	default:
		assert(false);
	}
}