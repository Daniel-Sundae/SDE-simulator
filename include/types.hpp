#pragma once
#include <vector>
#include <optional>
#include <future>
#include <stdexcept>
#include <cstddef>
#include <cassert>
#include <cstdint>

using Time = double;
using State = double;
using Density = double;
using Path = std::vector<State>;
using Paths = std::vector<Path>;
using Distribution = std::vector<State>;
using PDFData = std::vector<Density>;
using StateDot = double; // dX/dt
using Range = std::pair<double, double>;
using Task = std::packaged_task<Path()>;
template <typename T>
concept IntOrDouble = std::same_as<T, int> || std::same_as<T, double>;

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

