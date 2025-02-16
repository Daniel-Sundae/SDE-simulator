#include "InputHandler.hpp"
#include "ProcessData.hpp"
#include <assert.h>

InputHandler::InputHandler()
	: m_processType(ProcessType::NONE)
	, m_processDefinitionInputs(std::make_unique<ProcessDefinitionInputs>())
	, m_simParams(SimulationParameters())
	, m_driftGenerator(nullptr)
	, m_diffusionGenerator(nullptr)
{}

auto InputHandler::OnProcessButtonPressed(const ProcessType processType) -> void
{
    m_processType = processType;
    switch (processType){
	case ProcessType::NONE:
		throw std::invalid_argument("Not implemented yet");
	case ProcessType::CUSTOM:
		m_driftGenerator = nullptr;
		m_diffusionGenerator = nullptr;
		break;
    case ProcessType::BM:
        m_driftGenerator = [](double) { return ProcessData::BM::Drift(); }; // BM ignores mu
		m_diffusionGenerator = [](double) { return ProcessData::BM::Diffusion(); }; // BM ignores sigma
        break;
    case ProcessType::GBM:
        m_driftGenerator = ProcessData::GBM::Drift;
        m_diffusionGenerator = ProcessData::GBM::Diffusion;
        break;
	default:
		assert(false);
    }
    SamplePath();
}

auto InputHandler::OnProcessDefinitionModified(const ModifiedDefinitionParam param, double userValue) -> void
{
	switch (param) {
	case ModifiedDefinitionParam::MU:
		m_processDefinitionInputs->mu = userValue;
		break;
	case ModifiedDefinitionParam::SIGMA:
		m_processDefinitionInputs->sigma = userValue;
		break;
	case ModifiedDefinitionParam::STARTVALUE:
		m_processDefinitionInputs->startValue = userValue;
		break;
	default:
		assert(false);
	}
	if (m_processType != ProcessType::NONE) {
		SamplePath();
	}
}

auto InputHandler::OnSimulationParametersModified(SimulationParameters simParams) -> void
{
	m_simParams = simParams;
}

auto InputHandler::GetProcessDefinition() const -> ProcessDefinition
{
	auto drift = m_driftGenerator(m_processDefinitionInputs->mu);
	auto diffusion = m_diffusionGenerator(m_processDefinitionInputs->sigma);
	State startValue = m_processDefinitionInputs->startValue;
	return {drift, diffusion, startValue};
}
auto InputHandler::SamplePath() -> void
{
	if (m_processDefinitionInputs->mu == 0 && m_processDefinitionInputs->sigma == 0)
		return;
	if (m_processType != ProcessType::NONE)
		return;
	if (m_simParams.points <= 0 || m_simParams.time <= 0)
		return;

	Listener()->SamplePath({ m_processType, GetProcessDefinition(), m_simParams });
}
