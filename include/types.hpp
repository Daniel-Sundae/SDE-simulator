#pragma once
#include <vector>
#include <stdexcept>
#include <cstddef>

using Time = double;
using State = double;
using Path = std::vector<double>;
using Drift = std::function<State(Time, State)>;
using Diffusion = std::function<State(Time, State)>;
enum class ProcessType{
    NONE = 0,
    CUSTOM,
    BM,
    GBM,
    Levy,
    OU
};

struct ProcessDefinition {
    Drift mu = 0;
    Diffusion sigma = 0;
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

struct PathEngineQuery {
    ProcessType processType;
    ProcessDefinition processDefinition;
    SimulationParameters simulationParameters;
};