#include "InputHandler.hpp"
#include "ProcessData.hpp"
#include <assert.h>


InputHandler::InputHandler()
	: m_processDefinition(std::make_unique<ProcessDefinition>())
	, m_simulationParameters(std::make_unique<SimulationParameters>())
	, m_inputMu(DefinitionDefault::mu)
	, m_inputSigma(DefinitionDefault::sigma)
{}

auto InputHandler::OnProcessTypeModified(ProcessType newType) -> void
{
	m_processDefinition->type = newType;
	SamplePath();
}

auto InputHandler::OnProcessDefinitionModified(const ModifiedDefinitionParam param, double userValue) -> void
{
	switch (param) {
	case ModifiedDefinitionParam::PROCESS:
		throw std::invalid_argument("Use OnProcessTypeModified");
		break;
	case ModifiedDefinitionParam::MU:
		m_inputMu = userValue;
		break;
	case ModifiedDefinitionParam::SIGMA:
		m_inputSigma = userValue;
		break;
	case ModifiedDefinitionParam::STARTVALUE:
		m_processDefinition->startValue = userValue;
		break;
	default:
		assert(false);
	}
	SamplePath();
}

auto InputHandler::OnSimulationParametersModified(const SimulationParameters& simParams) -> void
{
	m_simulationParameters = std::make_unique<SimulationParameters>(simParams);
}

auto InputHandler::SamplePath() -> void
{
	if (m_processDefinition->type == ProcessType::NONE)
		return;
	if (m_inputMu == 0 && m_inputSigma == 0)
		return;
	if (m_simulationParameters->dt <= 0 || m_simulationParameters->time <= 0)
		return;

	// Need to get drift and diffusion here since they are dependent on type
	m_processDefinition = std::make_unique<ProcessDefinition>(
		m_processDefinition->type,
		ProcessData::GetDrift(m_processDefinition->type, m_inputMu),
		ProcessData::GetDiffusion(m_processDefinition->type, m_inputSigma),
		m_processDefinition->startValue);

	Listener()->SamplePath({*m_processDefinition, *m_simulationParameters});
	
}
