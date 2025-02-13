#pragma once
#include "Types.hpp"
#include "MainPresenter.hpp"
#include <memory>

enum class ModifiedParam {
    MU = 0,
    SIGMA,
    STARTVALUE
};

class InputHandler final : public IPresenterComponent<MainPresenter> {
public:
    explicit InputHandler();
    auto OnProcessButtonPressed(const ProcessType processType) -> void;
    auto OnProcessDefinitionModified(ModifiedParam param, double userValue) -> void;
    auto OnSimulationParametersModified(SimulationParameters& simParams) -> void;

private:
    auto ProcessDefinition() -> ProcessDefinition;
    auto SamplePath() -> void;

private:
    struct ProcessDefinitionInputs {
        double mu;
        double sigma;
        double startValue;
    };
    ProcessType m_processType;
    std::unique_ptr<ProcessDefinitionInputs> m_processDefinitionInputs;
    std::unique_ptr<SimulationParameters> m_simParams;
    std::function<Drift(double)> m_driftGenerator;
    std::function<Diffusion(double)> m_diffusionGenerator;
};