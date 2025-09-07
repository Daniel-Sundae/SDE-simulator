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

struct DefinitionParameters {
    ProcessType type;
    Drift drift;
    Diffusion diffusion;
    State X0;

    explicit DefinitionParameters(ProcessType pt = DefaultConstants::process)
        : type(pt)
        , drift(getField(FieldTags::drift{}, pt, getField(FieldTags::muData{}, pt).defaultValue))
        , diffusion(getField(FieldTags::diffusion{}, pt, getField(FieldTags::sigmaData{}, pt).defaultValue))
        , X0(getField(FieldTags::X0{}, pt).defaultValue)
    {}
    explicit DefinitionParameters(ProcessType t, Drift d, Diffusion diff, State start)
        : type(t)
        , drift(d)
        , diffusion(diff)
        , X0(start)
    {
    }
};

struct SettingsParameters{
    bool useThreading = true;
    std::optional<size_t> seed = std::nullopt;
};

struct PathQuery {
    DefinitionParameters definitionParameters;
    SimulationParameters simulationParameters;
    SettingsParameters settingsParameters;
    PathQuery(ProcessType pt = DefaultConstants::process)
        : definitionParameters(DefinitionParameters(pt)),
          simulationParameters(SimulationParameters()),
          settingsParameters(SettingsParameters())
    {}
    PathQuery(DefinitionParameters def, SimulationParameters simParam, SettingsParameters settParam)
        : definitionParameters(std::move(def))
        , simulationParameters(std::move(simParam))
        , settingsParameters(std::move(settParam))
    {}
};

struct StochasticQuery : PathQuery {
    using PathQuery::PathQuery;
};
struct DeterministicQuery : PathQuery {
    using PathQuery::PathQuery;
    explicit DeterministicQuery(ProcessType pt = DefaultConstants::process)
        : PathQuery(deterministicDefinitionParameters(DefinitionParameters(pt)),
                    deterministicSimulationParameters(SimulationParameters()),
                    SettingsParameters()) {}
    explicit DeterministicQuery(const PathQuery& query)
        : PathQuery(deterministicDefinitionParameters(query.definitionParameters),
                    deterministicSimulationParameters(query.simulationParameters),
                    query.settingsParameters) {}

    DefinitionParameters deterministicDefinitionParameters(const DefinitionParameters& sQuery) const{
        return DefinitionParameters(
            sQuery.type,
            sQuery.drift,
            // Diffusion function is not needed by deterministic query
            {0, [](Time, State){return 0;}},
            sQuery.X0);
    }

    SimulationParameters deterministicSimulationParameters(const SimulationParameters& sQuery) const{
        return SimulationParameters(
            sQuery.solver,
            sQuery.time,
            sQuery.dt,
            // Only need one sample since deterministic
            1);
    }
};
struct StochasticFullPathsQuery : PathQuery {
    using PathQuery::PathQuery;
    explicit StochasticFullPathsQuery()
        : PathQuery(DefinitionParameters(),
                    stochasticFullPathsSimulationParameters(SimulationParameters()),
                    SettingsParameters()) {}
    explicit StochasticFullPathsQuery(const StochasticQuery& sQuery)
        : PathQuery(sQuery.definitionParameters,
                    stochasticFullPathsSimulationParameters(sQuery.simulationParameters),
                    sQuery.settingsParameters) {}

    SimulationParameters stochasticFullPathsSimulationParameters(const SimulationParameters& sQuery) const{
        return SimulationParameters(
            sQuery.solver,
            sQuery.time,
            sQuery.dt,
            // Only drawn paths are kept as full paths
            std::min(sQuery.samples, DefaultConstants::maxPathsToDraw));
    }
};
using AnyQuery = std::variant<DeterministicQuery, StochasticQuery, StochasticFullPathsQuery>;

