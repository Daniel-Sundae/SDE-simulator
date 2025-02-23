#pragma once
#include "BasicTypes.hpp"
namespace SimulationDefault {
    constexpr Time time = 100;
    constexpr Time dt = 0.1;
    constexpr std::size_t samples = 1;
}

namespace DefinitionDefault {
    constexpr double mu = 0.1;
    constexpr double sigma = 0.1;
    constexpr State startValue = 0;
}