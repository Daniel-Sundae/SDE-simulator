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

// Remove const
auto MainPresenter::SamplePaths(const PathQuery& pQuery) -> void
{
	//Listener()->SamplingStarted(); //Setup loading symbol
	m_engine->SamplePathsAsync(
		pQuery,
		[this, pQuery](Paths results) {
			Listener()->HandleWorkerResult(pQuery, std::move(results));
		}
	);
}

// Remove const
auto MainPresenter::SampleDriftCurve(const PathQuery& deterministicQuery) -> void
{
	assert(deterministicQuery.simulationParameters.samples == 1);
	Listener()->OnDriftDataReceived(m_engine->SampleDriftCurve(deterministicQuery));
}

// Remove const
auto MainPresenter::OnQueriesReceived(const PathQuery& pQuery, const PathQuery& deterministicQuery, const PDFQuery& pdfQuery) -> void
{
	if(m_engine->IsBusy()){
		return;
	}
	SamplePaths(pQuery);
	SampleDriftCurve(deterministicQuery);
	GeneratePDFData(pdfQuery);
}

// Remove const
auto MainPresenter::GeneratePDFData(const PDFQuery& pdfQuery) -> void
{
	// This is really confusing pattern that I should change.
	// Looks like result is ignored and not clear that pdf is caching data
	m_engine->GeneratePDFData(pdfQuery);
	Listener()->OnPDFReceived(pdfQuery.pdf);
}

// Remove const
auto MainPresenter::Clear() -> void
{
	Listener()->OnClear();
	// m_engine->RequestCancel();
}

auto MainPresenter::GetInputHandler() const -> InputHandler*
{
	return m_inputHandler.get();
}
auto MainPresenter::GetOutputHandler() const -> OutputHandler*
{
	return m_outputHandler.get();
}
