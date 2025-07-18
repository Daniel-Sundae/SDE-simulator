#include "InputHandler.hpp"
#include "ProcessData.hpp"
#include "Transaction.hpp"
#include "Utils.hpp"
#include <assert.h>

InputHandler::InputHandler()
    : IPresenterComponent()
    , m_processDefinition(std::make_unique<ProcessDefinition>())
    , m_simulationParameters(std::make_unique<SimulationParameters>())
    , m_settingsParameters(std::make_unique<SettingsParameters>())
{
    m_inputMu = ProcessData::getMuData(m_processDefinition->type).defaultValue;
    m_inputSigma = ProcessData::getSigmaData(m_processDefinition->type).defaultValue;
}

void InputHandler::clear() const{
    listener()->clear();
}

void InputHandler::cancel() const{
    listener()->cancel();
}

bool InputHandler::canSample() const{
    if (m_processDefinition->type == ProcessType::NONE)
        return false;
    if (m_inputMu == 0 && m_inputSigma == 0)
        return false;
    if (m_simulationParameters->dt <= 0 || m_simulationParameters->time <= 0)
        return false;
    return true;
}

void InputHandler::samplePaths(){
    if(!canSample())
        return;
    m_processDefinition = std::make_unique<ProcessDefinition>(
        m_processDefinition->type,
        ProcessData::getDrift(m_processDefinition->type, m_inputMu),
        ProcessData::getDiffusion(m_processDefinition->type, m_inputSigma),
        m_processDefinition->startValueData);

    const PathQuery pQuery{ *m_processDefinition, *m_simulationParameters, *m_settingsParameters};
    const PathQuery deterministicQuery = createDriftQuery(pQuery);
    const PDF pdf = ProcessData::getPDF(pQuery.processDefinition.type, pQuery.processDefinition.startValueData, pQuery.simulationParameters.time, pQuery.processDefinition.drift.mu(), pQuery.processDefinition.diffusion.sigma());
    listener()->onTransactionReceived(Transaction{std::move(pQuery), std::move(deterministicQuery), std::move(pdf)});
}

PathQuery InputHandler::createDriftQuery(const PathQuery& pQuery) const{
    auto definition = ProcessDefinition(
        pQuery.processDefinition.type,
        pQuery.processDefinition.drift,
        // Diffusion function is not needed by deteministic query
        {0, [](Time, State){return 0;}},
        pQuery.processDefinition.startValueData);
    auto simulationParams = SimulationParameters(
        pQuery.simulationParameters.solver,
        pQuery.simulationParameters.time,
        pQuery.simulationParameters.dt,
        1);
    return PathQuery( definition, simulationParams, pQuery.settingsParameters);
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
        m_processDefinition->startValueData = userValue;
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
                m_settingsParameters->useSeed = {false, 0};
            } else {
                m_settingsParameters->useSeed = {true, static_cast<size_t>(userValue)};
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