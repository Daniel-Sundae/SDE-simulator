#include "InputHandler.hpp"

InputHandler::InputHandler()
	: m_processType(NONE)
	, m_params({})
	, m_simParams({10, 1000}
	, m_listener(nullptr))
{}

auto InputHandler::OnProcessButtonPressed(ProcessType processType) -> void
{
	m_processType = process;
	SamplePath();
}

{
auto InputHandler::OnProcessParametersModified(ModifiedParam param, double value) -> void
{
	switch (param) {
	case MU:
		
	}
	if (m_processType != NONE) {
		SamplePath();
	}
}



static inline auto Create(ProcessType type) -> ProcessMetaData {
	switch (type) {
	case ProcessType::BM:
		return ProcessMetaData(
			QString("Brownian Motion"),
			QString("BM"),
			QString(

auto InputHandler::OnSimulationParametersChanged(SimulationParameters& simParams) -> void
{
	m_simParams = std::make_unique<SimulationParameters>(std::move(simParams));
	//simParams.reset(simParams);
}

auto InputHandler::SamplePath() -> void
{
	static_assert(m_processType != NONE);
	static_assert(m_params);
	static_assert(m_simParams);
	m_listener->SamplePath(m_processType, m_params, m_simParams);
}

auto InputHandler::CreateQuery()

//auto InputHandler::SetListener(const MainPresenter* mainPresenter) -> void
//{
//	m_listener = mainPresenter;
//}