#include "MainPresenter.hpp"
#include "PathEngine.hpp"
#include "InputHandler.hpp"

MainPresenter::MainPresenter()
	: m_inputHandler(std::make_unique<InputHandler>())
	, m_outputHandler(std::make_unique<OutputHandler>())
{}

auto MainPresenter::SamplePath(const PathQuery& pathQuery) const -> void
{
	PathEngine engine{};
	Listener()->OnPathReceived(engine.SamplePath(pathQuery));
}

auto MainPresenter::GetInputHandler() const -> InputHandler*
{
	return m_inputHandler.get();
}
auto MainPresenter::GetOutputHandler() const -> OutputHandler*
{
	return m_outputHandler.get();
}
