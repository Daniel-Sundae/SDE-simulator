#pragma once
#include "ProcessData.hpp"
#include "Constants.hpp"
#include <optional>
struct SimulationParameters {
    explicit SimulationParameters() = default;
    explicit SimulationParameters(SolverType _solver, Time _time, Time _dt, size_t _samples)
        : solver(_solver)
        , time(_time)
        , dt(_dt)
        , samples(_samples)
    {
        assertValidParameters();
    }
    void assertValidParameters() const{
        if (dt <= 0 || time <= 0) {
            throw std::invalid_argument("Time and points must be greater than 0");
        }
    }
    [[nodiscard]] size_t points() const{
        return static_cast<size_t>(std::ceil(time / dt));
    }
    SolverType solver = DefaultConstants::Simulation::solver;
    Time time = DefaultConstants::Simulation::time;
    Time dt = DefaultConstants::Simulation::dt;
    size_t samples = DefaultConstants::Simulation::samples;
};

struct ProcessDefinition {
    ProcessType type = DefaultConstants::process;
    Drift drift = getField(FieldTags::drift{}, DefaultConstants::process, 0.0);
    Diffusion diffusion = getField(FieldTags::diffusion{}, DefaultConstants::process, 0.0);
    State startValueData = getField(FieldTags::startValueData{}, DefaultConstants::process).defaultValue;

    explicit ProcessDefinition() = default;

    explicit ProcessDefinition(ProcessType t, Drift d, Diffusion diff, State start)
        : type(t)
        , drift(d)
        , diffusion(diff)
        , startValueData(start)
    {
    }
};

struct SettingsParameters{
    bool useThreading = true;
    std::optional<size_t> useSeed = std::nullopt;
};

struct PathQuery {
    const ProcessDefinition processDefinition;
    const SimulationParameters simulationParameters;
    const SettingsParameters settingsParameters;
    PathQuery(const ProcessDefinition& def, const SimulationParameters& simParam, const SettingsParameters& settParam)
        : processDefinition(def)
        , simulationParameters(simParam)
        , settingsParameters(settParam)
    {
    }
};
