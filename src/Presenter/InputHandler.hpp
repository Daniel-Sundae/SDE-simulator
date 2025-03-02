#pragma once
#include "Types.hpp"
#include "DefaultConstants.hpp"
#include "PathQuery.hpp"
#include "MainPresenter.hpp"
#include <memory>


class InputHandler final : public IPresenterComponent<MainPresenter> {
public:
    explicit InputHandler();
    auto OnProcessTypeModified(ProcessType newType) -> void;
    auto OnProcessDefinitionModified(DefinitionWidget param, double userValue) -> void;
    auto OnSolverTypeModified(SolverType newType) -> void;
    template <IntOrDouble T>
    auto OnSimulationParametersModified(const SimulationWidget param, T userValue) -> void;
    auto SamplePath() -> void;
    auto Clear() -> void;
private:
    auto CanSample() const -> bool;
private:
    std::unique_ptr<ProcessDefinition> m_processDefinition;
    std::unique_ptr<SimulationParameters> m_simulationParameters;
    double m_inputMu;
    double m_inputSigma;
};

#include "InputHandler.inl"