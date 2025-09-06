#include "InputHandler.hpp"
#include "ProcessData.hpp"
#include "Transaction.hpp"
#include "Utils.hpp"

InputHandler::InputHandler(MainPresenter& mainPresenter)
    : m_mainPresenter(mainPresenter)
    , m_definitionParameters(std::make_unique<DefinitionParameters>())
    , m_simulationParameters(std::make_unique<SimulationParameters>())
    , m_settingsParameters(std::make_unique<SettingsParameters>())
{
}

void InputHandler::clear() const{
    m_mainPresenter.clear();
}

void InputHandler::cancel() const{
    m_mainPresenter.cancel();
}

void InputHandler::samplePaths(){
    const StochasticQuery stochasticQuery{ *m_definitionParameters, *m_simulationParameters, *m_settingsParameters};
    const DeterministicQuery deterministicQuery(stochasticQuery);
    const StochasticFullPathsQuery stochasticFullPathQuery(stochasticQuery);
    m_mainPresenter.onTransactionReceived(
        Transaction{std::move(deterministicQuery), std::move(stochasticQuery), std::move(stochasticFullPathQuery)});
}

void InputHandler::onSolverTypeModified(SolverType newType){
    m_simulationParameters->solver = newType;
}

void InputHandler::onProcessTypeModified(ProcessType newType){
    m_definitionParameters->type = newType;
    // Swap out the drift functions to use the new process type
    m_definitionParameters->drift = getField(FieldTags::drift{}, newType, m_definitionParameters->drift.mu());
    m_definitionParameters->diffusion = getField(FieldTags::diffusion{}, newType, m_definitionParameters->diffusion.sigma());
}

void InputHandler::onDefinitionParametersModified(const DefinitionWidget param, double userValue){
    switch (param) {
    case DefinitionWidget::PROCESS:
        throw std::invalid_argument("Use OnProcessTypeModified");
        break;
    case DefinitionWidget::MU:
        m_definitionParameters->drift = getField(FieldTags::drift{}, m_definitionParameters->type, userValue);
        break;
    case DefinitionWidget::SIGMA:
        m_definitionParameters->diffusion = getField(FieldTags::diffusion{}, m_definitionParameters->type, userValue);
        break;
    case DefinitionWidget::X0:
        m_definitionParameters->X0 = userValue;
        break;
    default:
        Utils::fatalError("Modifiying DefinitionWidget parameter: {} is not handled", static_cast<int>(param));
        break;
    }
}

void InputHandler::onSimulationParametersModified(const SimulationWidget param, int userValue) {
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

void InputHandler::onSimulationParametersModified(const SimulationWidget param, double userValue) {
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