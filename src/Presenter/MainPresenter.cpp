#include "MainPresenter.hpp"
#include "PathEngine.hpp"
#include "InputHandler.hpp"
#include "PDFQuery.hpp"
#include <cassert>

MainPresenter::MainPresenter()
	: IPresenterComponent()
	, m_inputHandler(std::make_unique<InputHandler>())
	, m_outputHandler(std::make_unique<OutputHandler>())
	, m_engine(std::make_unique<PathEngine>())
{}

auto MainPresenter::SamplePaths(const PathQuery& pQuery) const -> void
{
	//Listener()->SamplingStarted(); //Setup loading symbol
	Listener()->OnPathsReceived(pQuery, m_engine->SamplePaths(pQuery));
}

auto MainPresenter::GetDrift(const PathQuery& pQuery) const -> void
{
	assert(pQuery.simulationParameters.samples == 1);
	Listener()->OnDriftDataReceived(m_engine->SamplePaths(pQuery)[0]);
}

auto MainPresenter::GeneratePDFData(const PDFQuery& pdfQuery) const -> void
{
	// This is really confusing pattern that I should change.
	// Looks like result is ignored and not clear that pdf is caching data
	m_engine->GeneratePDFData(pdfQuery);
	Listener()->OnPDFReceived(pdfQuery.pdf);
}

auto MainPresenter::Clear() const -> void
{
	Listener()->OnClear();
}

auto MainPresenter::GetInputHandler() const -> InputHandler*
{
	return m_inputHandler.get();
}
auto MainPresenter::GetOutputHandler() const -> OutputHandler*
{
	return m_outputHandler.get();
}
