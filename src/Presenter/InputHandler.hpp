#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include "Constants.hpp"
#include "MainPresenter.hpp"
#include <memory>
#include <optional>


class InputHandler final : public IPresenterComponent<MainPresenter> {
public:
    explicit InputHandler();
    void onProcessTypeModified(ProcessType newType);
    void onProcessDefinitionModified(DefinitionWidget param, double userValue);
    void onSolverTypeModified(SolverType newType);
    template <IntOrDouble T> void onSimulationParameterModified(const SimulationWidget param, T userValue);
    template <IntOrBool T> void onSettingsParameterModified(const SettingsWidget param, T userValue);
    void samplePaths();
    void clear() const;
    void cancel() const;
private:
    bool canSample() const;
    PathQuery createDriftQuery(const PathQuery& pQuery) const;
private:
    std::unique_ptr<ProcessDefinition> m_processDefinition;
    std::unique_ptr<SimulationParameters> m_simulationParameters;
    std::unique_ptr<SettingsParameters> m_settingsParameters;
    double m_inputMu;
    double m_inputSigma;
};

#include "InputHandler.inl"