#pragma once
#include <vector>
#include <stdexcept>
#include <functional>
#include <cstddef>
#include "DefaultConstants.hpp"

class FunctionWrapper{
protected:
    const double parameter;
    std::function<State(Time, State)> f;
    FunctionWrapper(double inp_parameter, std::function f)
        : parameter(inp_parameter) {
    }

public:
    State operator()(Time t, State s) { return f(t, s); }
    double getParameter() const { return parameter; }
};

class Drift : public FunctionWrapper {
public:
    Drift(double mu_val) : FunctionWrapper(mu_val) {
        f = [mu_val](Time t, State s) { return mu_val * s; };
    }
    auto Mu() const -> double { return parameter; }
};

class Diffusion : public FunctionWrapper {
public:
    Diffusion(double sigma_val) : FunctionWrapper(sigma_val) {
        f = [sigma_val](Time t, State s) { return sigma_val * s; };
    }
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