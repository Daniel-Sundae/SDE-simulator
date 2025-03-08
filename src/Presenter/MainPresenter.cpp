#include "MainPresenter.hpp"
#include "PathEngine.hpp"
#include "InputHandler.hpp"
#include <cassert>

MainPresenter::MainPresenter()
	: IPresenterComponent()
	, m_inputHandler(std::make_unique<InputHandler>())
	, m_outputHandler(std::make_unique<OutputHandler>())
{}

auto MainPresenter::SamplePaths(const PathQuery& pQuery) const -> void
{
	PathEngine engine{};
	Listener()->OnPathsReceived(pQuery, engine.SamplePaths(pQuery));
}

auto MainPresenter::GetDrift(const PathQuery& pQuery) const -> void
{
	assert(pQuery.simulationParameters.samples == 1);
	PathEngine engine{};
	Listener()->OnDriftLineReceived(engine.SamplePaths(pQuery)[0]);
}

auto MainPresenter::GetPDFData(const PDFQuery& pdfQuery) const -> void
{
	PathEngine engine{};
	(void)pdfQuery;
	//Listener()->OnDriftLineReceived(engine.PDFData(pdfQuery));
}

auto MainPresenter::Clear() const -> void
{
	m_outputHandler->OnClear();
}

// TODO: Do i need these two?
auto MainPresenter::GetInputHandler() const -> InputHandler*
{
	return m_inputHandler.get();
}
auto MainPresenter::GetOutputHandler() const -> OutputHandler*
{
	return m_outputHandler.get();
}
