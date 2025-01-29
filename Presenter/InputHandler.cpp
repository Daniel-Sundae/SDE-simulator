#include "InputHandler.hpp"

InputHandler::InputHandler()
	: m_processType(NONE)
	, m_def({})
{}

auto InputHandler::OnProcessButtonPressed(ProcessType processType) -> void
{
	m_processType = process;
	SamplePath();
}

auto InputHandler::OnParametersChanged(ProcessDefinition& def) -> void
{
	m_def = def;
}

auto InputHandler::SamplePath() -> void
{

}