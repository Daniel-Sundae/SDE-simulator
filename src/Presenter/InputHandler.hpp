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
    void onDefinitionParametersModified(DefinitionWidget param, double userValue);
    void onSolverTypeModified(SolverType newType);
    void onSimulationParametersModified(const SimulationWidget param, int userValue);
    void onSimulationParametersModified(const SimulationWidget param, double userValue);
    void onSettingsParameterModified(const SettingsWidget param, int userValue);
    void onSettingsParameterModified(const SettingsWidget param, bool userValue);
    void samplePaths();
    void clear() const;
    void cancel() const;
private:
    MainPresenter& m_mainPresenter;
    std::unique_ptr<DefinitionParameters> m_definitionParameters;
    std::unique_ptr<SimulationParameters> m_simulationParameters;
    std::unique_ptr<SettingsParameters> m_settingsParameters;
};