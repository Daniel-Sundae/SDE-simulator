#pragma once
#include "Types.hpp"

class InputHandler{
public:
    explicit InputHandler();
    auto OnProcessButtonPressed(ProcessType processType) -> void;
    auto OnProcessParametersChanged(ProcessParameters& params) -> void;
    auto OnSimulationParametersChanged(SimulationParameters& simParams) -> void;
private:
    auto SamplePath() -> void;

private:
    ProcessType m_processType;
    ProcessParameters m_params;
    SimulationParameters m_simParams;
}