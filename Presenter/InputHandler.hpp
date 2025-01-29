#pragma once
#include "Types.hpp"

class InputHandler{
public:
    explicit InputHandler();
    auto OnProcessButtonPressed(ProcessType processType) -> void;
    auto OnParametersChanged(ProcessDefinition& def) -> void;
    // auto OnSamplingChanged(ProcessSampler sampler) -> void;
private:
    auto SamplePath() -> void;

private:
    ProcessType m_processType;
    ProcessDefinition m_def;
}