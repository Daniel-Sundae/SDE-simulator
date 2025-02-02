#include "InputHandler.hpp"

InputHandler::InputHandler()
	: m_processType(NONE)
	, m_def({})
	, m_simulationParameters({10, 1000})
{}

auto InputHandler::OnProcessButtonPressed(ProcessType processType) -> void
{
	m_processType = process;
	SamplePath();
}

auto InputHandler::OnProcessParametersChanged(ProcessParameters& params) -> void
{
	m_params = params;
	if (m_processType) {
		SamplePath();
	}
}

auto InputHandler::OnSimulationParametersChanged(SimulationParameters& simParams) -> void
{
	m_simParams = simParams;
}

auto InputHandler::SamplePath() -> void
{

}