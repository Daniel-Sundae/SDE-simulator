#include "InputHandler.hpp"
#include "ProcessData.hpp"
#include "Transaction.hpp"
#include "Utils.hpp"

InputHandler::InputHandler(MainPresenter& mainPresenter)
    : m_mainPresenter(mainPresenter)
    , m_processDefinition(std::make_unique<ProcessDefinition>())
    , m_simulationParameters(std::make_unique<SimulationParameters>())
    , m_settingsParameters(std::make_unique<SettingsParameters>())
{
    m_inputMu = getField(FieldTags::muData{}, m_processDefinition->type).defaultValue;
    m_inputSigma = getField(FieldTags::sigmaData{}, m_processDefinition->type).defaultValue;
}

void InputHandler::clear() const{
    m_mainPresenter.clear();
}

void InputHandler::cancel() const{
    m_mainPresenter.cancel();
}

void InputHandler::samplePaths(){
    m_processDefinition = std::make_unique<ProcessDefinition>(
        m_processDefinition->type,
        getField(FieldTags::drift{}, m_processDefinition->type, m_inputMu),
        getField(FieldTags::diffusion{}, m_processDefinition->type, m_inputSigma),
        m_processDefinition->startValue);

    const StochasticQuery stochasticQuery{ *m_processDefinition, *m_simulationParameters, *m_settingsParameters};
    const DeterministicQuery deterministicQuery = createDriftQuery(stochasticQuery);
    const StochasticFullPathsQuery stochasticFullPathQuery = createStochasticFullPathQuery(stochasticQuery);
    m_mainPresenter.onTransactionReceived(
        Transaction{std::move(deterministicQuery), std::move(stochasticQuery), std::move(stochasticFullPathQuery)});
}

DeterministicQuery InputHandler::createDriftQuery(const StochasticQuery& stochasticQuery) const{
    auto definition = ProcessDefinition(
        stochasticQuery.processDefinition.type,
        stochasticQuery.processDefinition.drift,
        // Diffusion function is not needed by deterministic query
        {0, [](Time, State){return 0;}},
        stochasticQuery.processDefinition.startValue);
    auto simulationParams = SimulationParameters(
        stochasticQuery.simulationParameters.solver,
        stochasticQuery.simulationParameters.time,
        stochasticQuery.simulationParameters.dt,
        1);
    return DeterministicQuery( definition, simulationParams, stochasticQuery.settingsParameters);
}

StochasticFullPathsQuery InputHandler::createStochasticFullPathQuery(const StochasticQuery& stochasticQuery) const{
    auto simulationParams = SimulationParameters(
        stochasticQuery.simulationParameters.solver,
        stochasticQuery.simulationParameters.time,
        stochasticQuery.simulationParameters.dt,
        std::min(stochasticQuery.simulationParameters.samples, DefaultConstants::maxPathsToDraw)); // Only drawn paths are kept in full
    return StochasticFullPathsQuery( stochasticQuery.processDefinition, simulationParams, stochasticQuery.settingsParameters);
}

void InputHandler::onSolverTypeModified(SolverType newType){
    m_simulationParameters->solver = newType;
}

void InputHandler::onProcessTypeModified(ProcessType newType){
    m_processDefinition->type = newType;
}

void InputHandler::onProcessDefinitionModified(const DefinitionWidget param, double userValue){
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
        m_processDefinition->startValue = userValue;
        break;
    default:
        Utils::fatalError("Modifiying DefinitionWidget parameter: {} is not handled", static_cast<int>(param));
        break;
    }
}

void InputHandler::onSimulationParameterModified(const SimulationWidget param, int userValue) {
    switch (param) {
        case SimulationWidget::TIME:
            m_simulationParameters->time = userValue;
            break;
        case SimulationWidget::SAMPLES:
            m_simulationParameters->samples = userValue;
            break;
        default:
            Utils::fatalError("SimulationWidget parameter: {} does not expect int", static_cast<int>(param));
    }
}

void InputHandler::onSimulationParameterModified(const SimulationWidget param, double userValue) {
    switch (param) {
        case SimulationWidget::dt:
            m_simulationParameters->dt = userValue;
            break;
        default:
            Utils::fatalError("SimulationWidget parameter: {} does not expect double", static_cast<int>(param));
    }
}

void InputHandler::onSettingsParameterModified(const SettingsWidget param, int userValue) {
    switch (param) {
        case SettingsWidget::FIXSEED:
            if (userValue == 0) {
                m_settingsParameters->seed = std::nullopt;
            } else {
                m_settingsParameters->seed = static_cast<size_t>(userValue);
            }
            break;
        default:
            Utils::fatalError("SettingsWidget parameter: {} does not expect int", static_cast<int>(param));
    }
}

void InputHandler::onSettingsParameterModified(const SettingsWidget param, bool userValue) {
    switch (param) {
        case SettingsWidget::THREADS:
            m_settingsParameters->useThreading = userValue;
            break;
        default:
            Utils::fatalError("SettingsWidget parameter: {} does not expect bool", static_cast<int>(param));
    }
}