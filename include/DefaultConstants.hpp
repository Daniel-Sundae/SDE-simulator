#pragma once
#include "BasicTypes.hpp"
namespace SimulationDefault {
    constexpr Time time = 10;
    constexpr std::size_t points = 1000;
}

namespace DefinitionDefault {
    const Drift drift = [](Time, State) -> StateDot { return 0.0; };
    const Diffusion diffusion = [](Time, State) -> StateDot { return 0.0; };
    constexpr double mu = 0.2;
    constexpr double sigma = 0.2;
    constexpr State startValue = 0;
}