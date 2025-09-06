#pragma once
#include <vector>
#include <optional>
#include <future>
#include <stdexcept>
#include <cstddef>
#include <cstdint>

using Time = double;
using State = double;
using Density = double;
using Path = std::vector<State>;
using Paths = std::vector<Path>;
using Distribution = std::vector<State>;
using PDFData = std::vector<Density>;
using Range = std::pair<State, State>;
using Job = struct Job;
template <typename T>
concept IntOrDouble = std::same_as<T, int> || std::same_as<T, double>;

enum class ProcessType{
    NONE = 0,
    CUSTOM,
    BM,
    GBM,
    OU,
    Levy,
};

enum class SolverType {
    EULER_MARUYAMA = 0,
    RUNGE_KUTTA,
    MILSTEIN
};

enum class ActionWidget {
    GO = 0,
    CLEAR,
};

enum class DefinitionWidget {
    PROCESS = 0,
    MU,
    SIGMA,
    X0
};

enum class SimulationWidget {
    SOLVER = 0,
    TIME,
    dt,
    SAMPLES,
};

enum class SettingsWidget {
    THREADS = 0,
    FIXSEED,
};