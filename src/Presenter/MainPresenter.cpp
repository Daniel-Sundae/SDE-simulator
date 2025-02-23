#include "MainPresenter.hpp"
#include "PathEngine.hpp"
#include "InputHandler.hpp"

MainPresenter::MainPresenter()
	: IPresenterComponent()
	, m_inputHandler(std::make_unique<InputHandler>())
	, m_outputHandler(std::make_unique<OutputHandler>())
{}

auto MainPresenter::SamplePath(const PathQuery& query) const -> void
{
	PathEngine engine{};
	Listener()->OnPathReceived(query, engine.SamplePath(query));
}

auto MainPresenter::Clear() const -> void
{
	m_outputHandler->Clear();
}

// Do i need these two?
auto MainPresenter::GetInputHandler() const -> InputHandler*
{
	return m_inputHandler.get();
}
auto MainPresenter::GetOutputHandler() const -> OutputHandler*
{
	return m_outputHandler.get();
}
