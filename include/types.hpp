#pragma once
#include <vector>
#include <stdexcept>
#include <functional>
#include <cstddef>
#include "DefaultConstants.hpp"
class FunctionWrapper {
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
    OU,
    BB,
};

enum class SolverType {
    EULER_MARUYAMA = 0,
    RUNGE_KUTTA,
};

enum class ActionWidget {
    GO = 0,
    CLEAR,
};

enum class DefinitionWidget {
    PROCESS = 0,
    MU,
    SIGMA,
    STARTVALUE
};

enum class SimulationWidget {
    SOLVER = 0,
    TIME,
    dt,
    SAMPLES,
};

enum class SettingsWidget {
    THREADS = 0,
    AUTOUPDATE,
    AUTOCLEAR,
    FIXSEED,
};

struct SimulationParameters {
    explicit SimulationParameters() = default;
    explicit SimulationParameters(Time inp_time, Time inp_dt)
        : time(inp_time)
        , dt(inp_dt)
    {
        AssertValidParameters();
    }
    auto AssertValidParameters() const -> void
    {
        if (dt <= 0 || time <= 0) {
            throw std::invalid_argument("Time and points must be greater than 0");
        }
    }
    [[nodiscard]] auto Points() const -> std::size_t
    {
        return static_cast<std::size_t>(std::ceil(time / dt));
    }
    Time time = SimulationDefault::time;
    Time dt = SimulationDefault::dt;
    // TODO add nr samples (for more than one path)
    // TODO add SolverMethod enum
};

struct ProcessDefinition {
    ProcessType type = ProcessType::BM;
    Drift drift = Drift(0, [](Time, State) -> StateDot { return 0.0; });
    Diffusion diffusion = Diffusion(0, [](Time, State) -> StateDot { return 0.0; });
    State startValue = DefinitionDefault::startValue;

    ProcessDefinition() = default;
    ProcessDefinition(const ProcessType t, Drift d, Diffusion diff, const State start)
        : type(t)
        , drift(d)
        , diffusion(diff)
        , startValue(start)
    {}
};

struct PathQuery {
    const ProcessDefinition& processDefinition;
    const SimulationParameters& simulationParameters;
    PathQuery(const ProcessDefinition& def, const SimulationParameters& simParam)
        : processDefinition(def)
        , simulationParameters(simParam)
    {}
};