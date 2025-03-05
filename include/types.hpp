#pragma once
#include <vector>
#include <stdexcept>
#include <functional>
#include <cstddef>

using Time = double;
using State = double;
using Density = double;
using Path = std::vector<State>;
using Paths = std::vector<Path>;
using Distribution = std::vector<State>;
using PDF = std::vector<Density>; // Probability density function
using StateDot = double; // dX/dt
using Range = std::pair<double, double>;
template <typename T>
concept IntOrDouble = std::same_as<T, int> || std::same_as<T, double>;
class PathQuery;
class PDFQuery;
template <typename T>
concept Query = std::same_as<T, PathQuery> || std::same_as<T, PDFQuery>;

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

class PDF {
    const double EV;
    const double stddev;
    const std::function<Density(State)> pdf;
    PDF(double _EV, double _stddev, std::function<Density(State)> _pdf)
        : EV(_EV)
        , stddev(_stddev)
        , pdf(_pdf)
    {}
    auto operator()(State s) const -> Density { return pdf(s); }
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

