#include "MainPresenter.hpp"
#include "PathEngine.hpp"
#include "InputHandler.hpp"
#include "PDFQuery.hpp"
#include "ModelUtils.hpp"
#include <cassert>

MainPresenter::MainPresenter()
	: IPresenterComponent()
	, m_inputHandler(std::make_unique<InputHandler>())
	, m_outputHandler(std::make_unique<OutputHandler>())
	, m_engine(std::make_unique<PathEngine>())
{}

auto MainPresenter::SamplePaths(const PathQuery& pQuery) -> void
{
	m_engine->SamplePathsAsync(
		pQuery,
		[this](Paths results) mutable {
			Listener()->HandleWorkerResult(std::move(results));
		}
	);
}

auto MainPresenter::SampleDriftCurve(const PathQuery& deterministicQuery) -> void
{
	assert(deterministicQuery.simulationParameters.samples == 1);
	Listener()->OnDriftDataReceived(m_engine->SampleDriftCurve(deterministicQuery));
}

auto MainPresenter::OnTransactionReceived(const Transaction&& transaction) -> void
{
	if(m_engine->IsBusy()){
		return;
	}
	Listener()->PrepareGUI(transaction.pathQuery);
	// Need to set distribution support before sampling paths
	Listener()->OnPDFReceived(transaction.pdfQuery.pdf);
	SamplePaths(transaction.pathQuery);
	SampleDriftCurve(transaction.deterministicQuery);
}

auto MainPresenter::Clear() const -> void
{
	if(m_engine->IsBusy()){
		return;
	}
	Listener()->Clear();
}

auto MainPresenter::Cancel() const -> void
{
	m_engine->RequestCancel();
}

auto MainPresenter::GetInputHandler() const -> InputHandler*
{
	return m_inputHandler.get();
}
auto MainPresenter::GetOutputHandler() const -> OutputHandler*
{
	return m_outputHandler.get();
}
