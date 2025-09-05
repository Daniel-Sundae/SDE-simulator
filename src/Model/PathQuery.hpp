#pragma once
#include "ProcessData.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include <optional>
#include <variant>

struct SimulationParameters {
    explicit SimulationParameters() = default;
    explicit SimulationParameters(SolverType _solver, Time _time, Time _dt, size_t _samples)
        : solver(_solver)
        , time(_time)
        , dt(_dt)
        , samples(_samples)
    {
        Utils::assertTrue(dt > 0 && time > 0, "Time and points must be greater than 0");
    }
    size_t steps() const{
        return static_cast<size_t>(std::ceil(time / dt));
    }
    size_t nrPathsToDraw() const {
        return std::min(samples, DefaultConstants::maxPathsToDraw);
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
    State startValue = getField(FieldTags::startValue{}, DefaultConstants::process).defaultValue;

    explicit ProcessDefinition() = default;

    explicit ProcessDefinition(ProcessType t, Drift d, Diffusion diff, State start)
        : type(t)
        , drift(d)
        , diffusion(diff)
        , startValue(start)
    {
    }
};

struct SettingsParameters{
    bool useThreading = true;
    std::optional<size_t> seed = std::nullopt;
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

struct DeterministicQuery : PathQuery {
    using PathQuery::PathQuery;
};
struct StochasticQuery : PathQuery {
    using PathQuery::PathQuery;
};
struct StochasticFullPathsQuery : PathQuery {
    using PathQuery::PathQuery;
};
using AnyQuery = std::variant<DeterministicQuery, StochasticQuery, StochasticFullPathsQuery>;

