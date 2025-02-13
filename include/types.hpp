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
enum class ProcessType{
    NONE = 0,
    CUSTOM,
    BM,
    GBM,
    Levy,
    OU
};

struct ProcessDefinition {
    const Drift& drift = [](Time, State) -> State { return 0.0; };
    const Diffusion& diffusion = [](Time, State) -> State { return 0.0; };
    State startValue = 0;
};

struct SimulationParameters {
    explicit SimulationParameters(Time in_time, std::size_t in_points)
        : time(in_time), points(in_points)
    {
        if (points == 0 || time <= 0) {
            throw std::invalid_argument("Time and points must be greater than 0");
        }
    }
    auto dt() const -> Time
    {
        return time / static_cast<Time>(points);
    }
    Time time = 0;
    std::size_t points;
    // TODO add nr samples (for more than one path)
    // TODO add SolverMethod enum
};

struct PathQuery {
    const ProcessType processType;
    const ProcessDefinition& processDefinition;
    const SimulationParameters& simulationParameters;
};