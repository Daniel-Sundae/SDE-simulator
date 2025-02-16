#pragma once
#include "Types.hpp"
#include "MainPresenter.hpp"
#include <memory>

enum class ModifiedDefinitionParam {
    MU = 0,
    SIGMA,
    STARTVALUE
};

class InputHandler final : public IPresenterComponent<MainPresenter> {
public:
    explicit InputHandler();
    auto OnProcessButtonPressed(const ProcessType processType) -> void;
    auto OnProcessDefinitionModified(ModifiedDefinitionParam param, double userValue) -> void;
    auto OnSimulationParametersModified(SimulationParameters simParams) -> void;

private:
    auto GetProcessDefinition() const -> ProcessDefinition;
    auto SamplePath() -> void;

private:
    struct ProcessDefinitionInputs {
        double mu = 0;
        double sigma = 0;
        double startValue = 0;
    };
    ProcessType m_processType;
    std::unique_ptr<ProcessDefinitionInputs> m_processDefinitionInputs;
    SimulationParameters m_simParams;
    std::function<Drift(double)> m_driftGenerator;
    std::function<Diffusion(double)> m_diffusionGenerator;
};