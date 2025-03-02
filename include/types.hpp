#pragma once
#include <vector>
#include <stdexcept>
#include <functional>
#include <cstddef>

using Time = double;
using State = double;
using Path = std::vector<double>;
using StateDot = double; // dX/dt
using Range = std::pair<double, double>;
template <typename T>
concept IntOrDouble = std::same_as<T, int> || std::same_as<T, double>;

class FunctionWrapper {
protected:
    const double parameter;
    const std::function<StateDot(Time, State)> f;
public:
    FunctionWrapper()
        : parameter(0)
        , f([](Time, State) -> StateDot { return 0; })
    {}
    FunctionWrapper(double _parameter, std::function<StateDot(Time, State)> _f)
        : parameter(_parameter)
        , f(_f)
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
    FIXSEED,
};

