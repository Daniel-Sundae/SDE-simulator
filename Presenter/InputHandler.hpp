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
    auto OnProcessButtonPressed(ProcessType processType) -> void;
    auto OnProcessParametersModified(ModifiedParam param, double value) -> void;
    auto OnSimulationParametersModified(SimulationParameters& simParams) -> void;
    //auto SetListener(const MainPresenter* mainPresenter) -> void;

private:
    auto SamplePath() -> void;

private:
    ProcessType m_processType;
    std::unique_ptr<ProcessParameters> m_params;
    std::unique_ptr<SimulationParameters> m_simParams;
    const MainPresenter* m_listener;
}