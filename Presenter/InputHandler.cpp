#include "InputHandler.hpp"
#include <assert.h>

InputHandler::InputHandler()
	: m_processType(NONE)
	, m_processDefinitionInputs({})
	, m_simParams({10, 1000}
	, m_listener(nullptr)
	, m_driftGenerator(nullptr)
	, m_diffusionGenerator(nullptr))
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

auto InputHandler::OnProcessDefinitionModified(ModifiedParam param, double userValue) -> void
{
	switch (param) {
	case ModifiedParam::MU:
		m_processDefinitionInputs->mu = userValue;
		break;
	case ModifiedParam::SIGMA:
		m_processDefinitionInputs->sigma = userValue;
		break;
	case ModifiedParam::STARTVALUE:
		m_processDefinitionInputs->startValue = userValue;
		break;
	default:
		assert(false);
	}
	if (m_processType != ProcessType::NONE) {
		SamplePath();
	}
}

auto InputHandler::OnSimulationParametersModified(SimulationParameters& simParams) -> void
{
	m_simParams = std::make_unique<SimulationParameters>(std::move(simParams));
	//simParams.reset(simParams);
}

auto InputHandler::ProcessDefinition() -> ProcessDefinition
{
	ProcessDefinition def{};
	def.drift = m_driftGenerator(m_processDefinition.mu);
	def.diffusion = m_diffusionGenerator(m_processDefinition.sigma);
	def.startValue = m_processDefinition.startValue;
	return def;
}
auto InputHandler::SamplePath() -> void
{
	static_assert(m_processType != NONE);
	static_assert(m_processDefinitionInputs);
	static_assert(m_simParams);
	Listener()->SamplePath({ m_processType, ProcessDefinition(), m_simParams });
}

auto InputHandler::CreateQuery()

//auto InputHandler::SetListener(const MainPresenter* mainPresenter) -> void
//{
//	m_listener = mainPresenter;
//}