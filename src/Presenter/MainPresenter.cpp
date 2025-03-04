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

	// IDEA1: accumulate all paths into a vector and send it to outputhandler. Ther we search for the driftpath (how?)
	// IDEA2: create a result struct with the paths + the driftpath as std::optional.
	const Paths paths = engine.SamplePaths(query);
	Listener()->OnPathsReceived(query, paths);
	
	// If diffusion is non-zero we query for drift too
	// NOTE: need robust way of checking zero-ness of diffusion func. (sigma = 0 !=> Drift(state, time) = 0)
	// Example: Diffusion(t, X_t) = sqrt(abs(X_t)) which is independent of user sigma.
	// TODO This is temporary solution: Assume sigma = 0 => Diffusion = 0
	if (query.processDefinition.diffusion.Sigma() != 0 && query.processDefinition.drift.Mu() != 0){
		auto definition = ProcessDefinition(
			query.processDefinition.type,
			query.processDefinition.drift,
			{},
			query.processDefinition.startValueData);
		auto simulationParams = SimulationParameters(
			query.simulationParameters.solver,
			query.simulationParameters.time,
			query.simulationParameters.dt,
			1);
		PathQuery determenisticQuery({definition, simulationParams });
		Listener()->OnDriftLineReceived(engine.SamplePaths(determenisticQuery)[0]);
	}
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
