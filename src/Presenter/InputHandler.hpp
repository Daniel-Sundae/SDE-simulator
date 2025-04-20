#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include "Constants.hpp"
#include "MainPresenter.hpp"
#include <memory>
#include <optional>

struct PDFQuery;

class InputHandler final : public IPresenterComponent<MainPresenter> {
public:
    explicit InputHandler();
    auto OnProcessTypeModified(ProcessType newType) -> void;
    auto OnProcessDefinitionModified(DefinitionWidget param, double userValue) -> void;
    auto OnSolverTypeModified(SolverType newType) -> void;
    template <IntOrDouble T>
    auto OnSimulationParameterModified(const SimulationWidget param, T userValue) -> void;
    template <IntOrBool T>
    auto OnSettingsParameterModified(const SettingsWidget param, T userValue) -> void;
    auto SamplePaths() -> void;
    auto Clear() const -> void;
    auto Cancel() const -> void;
private:
    auto CanSample() const -> bool;
    auto CreateDriftQuery(const PathQuery& pQuery) const -> PathQuery;
    auto CreatePDFQuery(const PathQuery& pQuery) const -> PDFQuery;
private:
    std::unique_ptr<ProcessDefinition> m_processDefinition;
    std::unique_ptr<SimulationParameters> m_simulationParameters;
    std::unique_ptr<SettingsParameters> m_settingsParameters;
    double m_inputMu;
    double m_inputSigma;
};

#include "InputHandler.inl"