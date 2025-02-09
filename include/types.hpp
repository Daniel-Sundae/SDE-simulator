#pragma once
#include <vector>
#include <stdexcept>
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
    Drift drift = [](Time t, State s) -> State { return 0.0; };
    Diffusion diffusion = [](Time t, State s) -> State { return 0.0; };
    State startValue = 0;
};

struct SimulationParameters {
    explicit SimulationParameters(Time time, std::size_t points)
        : time(time), points(points)
    {
        if (points == 0 || time <= 0) {
            throw std::invalid_argument("Time and points must be greater than 0");
        }
    }
    auto dt() const -> Time
    {
        return time / points;
    }
    Time time = 0;
    std::size_t points;
};

struct PathQuery {
    const ProcessType processType;
    const ProcessDefinition& processDefinition;
    const SimulationParameters& simulationParameters;
};