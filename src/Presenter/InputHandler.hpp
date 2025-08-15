#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include "Constants.hpp"
#include "MainPresenter.hpp"
#include <memory>
#include <optional>

class InputHandler{
public:
    explicit InputHandler(MainPresenter& mainPresenter);
    void onProcessTypeModified(ProcessType newType);
    void onProcessDefinitionModified(DefinitionWidget param, double userValue);
    void onSolverTypeModified(SolverType newType);
    void onSimulationParameterModified(const SimulationWidget param, int userValue);
    void onSimulationParameterModified(const SimulationWidget param, double userValue);
    void onSettingsParameterModified(const SettingsWidget param, int userValue);
    void onSettingsParameterModified(const SettingsWidget param, bool userValue);
    void samplePaths();
    void clear() const;
    void cancel() const;
private:
    bool canSample() const;
    PathQuery createDriftQuery(const PathQuery& pQuery) const;
private:
    MainPresenter& m_mainPresenter;
    std::unique_ptr<ProcessDefinition> m_processDefinition;
    std::unique_ptr<SimulationParameters> m_simulationParameters;
    std::unique_ptr<SettingsParameters> m_settingsParameters;
    double m_inputMu;
    double m_inputSigma;
};