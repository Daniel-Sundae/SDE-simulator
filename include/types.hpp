#pragma once
#include <vector>
#include <stdexcept>
#include <functional>
#include <cstddef>

using Time = double;
using State = double;
using Path = std::vector<double>;
using StateDot = double; // dX/dt
using Drift = std::function<StateDot(Time, State)>;
using Diffusion = std::function<StateDot(Time, State)>;

namespace SimulationDefault {
    constexpr Time time = 10;
    constexpr std::size_t points = 1000;
}
namespace DefinitionDefault {
    const Drift drift = [](Time, State) -> StateDot { return 0.0; };
    const Diffusion diffusion = [](Time, State) -> StateDot { return 0.0; };
    constexpr State startValue = 0;
}

enum class ProcessType{
    NONE = 0,
    CUSTOM,
    BM,
    GBM,
    Levy,
    OU
};

struct SimulationParameters {
    explicit SimulationParameters() = default;
    explicit SimulationParameters(Time inp_time, std::size_t inp_points)
        : time(inp_time)
        , points(inp_points)
    {
        AssertValidParameters();
    }
    auto AssertValidParameters() const -> void
    {
        if (points == 0 || time <= 0) {
            throw std::invalid_argument("Time and points must be greater than 0");
        }
    }
    [[nodiscard]] auto dt() const -> Time
    {
        return time / static_cast<Time>(points);
    }
    Time time = SimulationDefault::time;
    std::size_t points = SimulationDefault::points;
    // TODO add nr samples (for more than one path)
    // TODO add SolverMethod enum
};

struct ProcessDefinition {
    const Drift drift = DefinitionDefault::drift;
    const Diffusion diffusion = DefinitionDefault::diffusion;
    State startValue = DefinitionDefault::startValue;

    ProcessDefinition(Drift d, Diffusion diff, const State start)
        : drift(std::move(d))
        , diffusion(std::move(diff))
        , startValue(start)
    {}
};

struct PathQuery {
    const ProcessType processType;
    const ProcessDefinition processDefinition;
    const SimulationParameters simulationParameters;
    PathQuery(const ProcessType type, ProcessDefinition def, SimulationParameters simParam)
        : processType(type)
        , processDefinition(def)
        , simulationParameters(simParam)
    {}
};