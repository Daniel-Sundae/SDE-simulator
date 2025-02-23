#include "InputHandler.hpp"
#include "ProcessData.hpp"
#include <assert.h>


InputHandler::InputHandler()
	: IPresenterComponent()
	, m_processDefinition(std::make_unique<ProcessDefinition>())
	, m_simulationParameters(std::make_unique<SimulationParameters>())
	, m_inputMu(DefinitionDefault::mu)
	, m_inputSigma(DefinitionDefault::sigma)
{}

auto InputHandler::Clear() -> void
{
	m_processDefinition = std::make_unique<ProcessDefinition>();
	m_simulationParameters = std::make_unique<SimulationParameters>();
	m_inputMu = DefinitionDefault::mu;
	m_inputSigma = DefinitionDefault::sigma;
	Listener()->Clear();
}

auto InputHandler::OnProcessTypeModified(ProcessType newType) -> void
{
	m_processDefinition->type = newType;
	SamplePath();
}

auto InputHandler::OnProcessDefinitionModified(const DefinitionWidget param, double userValue) -> void
{
	switch (param) {
	case DefinitionWidget::PROCESS:
		throw std::invalid_argument("Use OnProcessTypeModified");
		break;
	case DefinitionWidget::MU:
		m_inputMu = userValue;
		break;
	case DefinitionWidget::SIGMA:
		m_inputSigma = userValue;
		break;
	case DefinitionWidget::STARTVALUE:
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

auto InputHandler::CanSample() const -> bool
{
	if (m_processDefinition->type == ProcessType::NONE)
		return false;
	if (m_inputMu == 0 && m_inputSigma == 0)
		return false;
	if (m_simulationParameters->dt <= 0 || m_simulationParameters->time <= 0)
		return false;
	return true;
}

auto InputHandler::SamplePath() -> void
{
	if(!CanSample())
		return;

	// Need to get drift and diffusion here since they are dependent on type
	m_processDefinition = std::make_unique<ProcessDefinition>(
		m_processDefinition->type,
		ProcessData::GetDrift(m_processDefinition->type, m_inputMu),
		ProcessData::GetDiffusion(m_processDefinition->type, m_inputSigma),
		m_processDefinition->startValue);

	Listener()->SamplePath({*m_processDefinition, *m_simulationParameters});
	
}
