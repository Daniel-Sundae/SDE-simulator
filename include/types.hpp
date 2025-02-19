#pragma once
#include <vector>
#include <stdexcept>
#include <functional>
#include <cstddef>
#include "DefaultConstants.hpp"

class FunctionWrapper{
protected:
    const double parameter;
    const std::function<StateDot(Time, State)> f;
public:
    FunctionWrapper(double inp_parameter, std::function<StateDot(Time, State)> inp_f)
        : parameter(inp_parameter)
        , f(inp_f)
    {}
    auto operator()(Time t, State s) const -> StateDot { return f(t, s); }
};

class Drift : public FunctionWrapper {
    using FunctionWrapper::FunctionWrapper;
public:
    auto Mu() const -> double { return parameter; }
};

class Diffusion : public FunctionWrapper {
    using FunctionWrapper::FunctionWrapper;
public:
    auto Sigma() const -> double { return parameter; }
};

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
    const Drift drift = Drift(0, [](Time, State) -> StateDot { return 0.0; });
    const Diffusion diffusion = Diffusion(0, [](Time, State) -> StateDot { return 0.0; });
    State startValue = DefinitionDefault::startValue;

    ProcessDefinition(Drift d, Diffusion diff, const State start)
        : drift(d)
        , diffusion(diff)
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