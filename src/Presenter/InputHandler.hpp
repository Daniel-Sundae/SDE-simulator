#pragma once
#include "Types.hpp"
#include "DefaultConstants.hpp"
#include "MainPresenter.hpp"
#include <memory>

enum class ModifiedDefinitionParam {
    PROCESS = 0,
    MU,
    SIGMA,
    STARTVALUE
};

class InputHandler final : public IPresenterComponent<MainPresenter> {
public:
    explicit InputHandler();
    auto OnProcessTypeModified(ProcessType newType) -> void;
    auto OnProcessDefinitionModified(ModifiedDefinitionParam param, double userValue) -> void;
    auto OnSimulationParametersModified(const SimulationParameters& simParams) -> void;

private:
    auto SamplePath() -> void;

private:

    std::unique_ptr<ProcessDefinition> m_processDefinition;
    std::unique_ptr<SimulationParameters> m_simulationParameters;
    double m_inputMu;
    double m_inputSigma;
};