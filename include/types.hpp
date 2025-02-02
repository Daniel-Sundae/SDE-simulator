#pragma once
#include <vector>
#include <stdexcept>
#include <cstddef>

using Time = double;
using State = double;
using Path = std::vector<double>;
enum class ProcessType{
    NONE = 0,
    BM,
    GBM,
    Levy,
    OU
};

struct ProcessParameters {
    double mu = 0;
    double sigma = 0;
    double startValue = 0;
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
    double time = 0;
    std::size_t points;
};