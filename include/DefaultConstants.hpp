#pragma once
#include "Types.hpp"
namespace SimulationDefault {
    constexpr SolverType solver = SolverType::EULER_MARUYAMA;
    constexpr Time time = 10;
    constexpr Time dt = 0.01;
    constexpr std::size_t samples = 1;
}