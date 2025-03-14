#include "InputHandler.hpp"
#include "ProcessData.hpp"
#include "PDFQuery.hpp"
#include <assert.h>

InputHandler::InputHandler()
	: IPresenterComponent()
	, m_processDefinition(std::make_unique<ProcessDefinition>())
	, m_simulationParameters(std::make_unique<SimulationParameters>())
{
	m_inputMu = ProcessData::GetMuData(m_processDefinition->type).defaultValue;
	m_inputSigma = ProcessData::GetSigmaData(m_processDefinition->type).defaultValue;
}

auto InputHandler::Clear() -> void
{
	Listener()->Clear();
}

auto InputHandler::OnProcessTypeModified(ProcessType newType) -> void
{
	m_processDefinition->type = newType;
}

auto InputHandler::OnSolverTypeModified(SolverType newType) -> void
{
	m_simulationParameters->solver = newType;
}

auto InputHandler::OnProcessDefinitionModified(const DefinitionWidget param, double userValue) -> void
{
	switch (param) {
	case DefinitionWidget::PROCESS:
		throw std::invalid_argument("Use OnProcessTypeModified");
		break;
	case DefinitionWidget::MU:
		m_inputMu = userValue;
		break;
	case DefinitionWidget::SIGMA:
		m_inputSigma = userValue;
		break;
	case DefinitionWidget::STARTVALUE:
		m_processDefinition->startValueData = userValue;
		break;
	default:
		assert(false);
	}
	SamplePaths();
}

auto InputHandler::CanSample() const -> bool
{
	if (m_processDefinition->type == ProcessType::NONE)
		return false;
	if (m_inputMu == 0 && m_inputSigma == 0)
		return false;
	if (m_simulationParameters->dt <= 0 || m_simulationParameters->time <= 0)
		return false;
	return true;
}

auto InputHandler::SamplePaths() -> void
{
	if(!CanSample())
		return;

	// Need to get drift and diffusion here since they are dependent on type
	m_processDefinition = std::make_unique<ProcessDefinition>(
		m_processDefinition->type,
		ProcessData::GetDrift(m_processDefinition->type, m_inputMu),
		ProcessData::GetDiffusion(m_processDefinition->type, m_inputSigma),
		m_processDefinition->startValueData);

	const PathQuery pQuery = PathQuery{ *m_processDefinition, *m_simulationParameters };
	const PathQuery deterministicQuery = CreateDriftQuery(pQuery);
	const PDFQuery pdfQuery = CreatePDFQuery(pQuery);
	Listener()->SamplePaths(pQuery);
	Listener()->GetDrift(deterministicQuery);
	Listener()->GeneratePDFData(pdfQuery);
}

auto InputHandler::CreateDriftQuery(const PathQuery& pQuery) const -> PathQuery {

	auto definition = ProcessDefinition(
		pQuery.processDefinition.type,
		pQuery.processDefinition.drift,
		{},
		pQuery.processDefinition.startValueData);
	auto simulationParams = SimulationParameters(
		pQuery.simulationParameters.solver,
		pQuery.simulationParameters.time,
		pQuery.simulationParameters.dt,
		1);
	return PathQuery( definition, simulationParams );
}

auto InputHandler::CreatePDFQuery(const PathQuery& pQuery) const -> PDFQuery {
	PDF pdf = ProcessData::GetPDF(pQuery.processDefinition.type, pQuery.processDefinition.startValueData, pQuery.simulationParameters.time, pQuery.processDefinition.drift.Mu(), pQuery.processDefinition.diffusion.Sigma());
	return PDFQuery(pdf, pQuery.simulationParameters.Points());
}

