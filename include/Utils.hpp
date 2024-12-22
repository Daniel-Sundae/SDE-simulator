#pragma once
#include <random>
#include <cmath>
#include "types.hpp"
#include "SDE.hpp"

namespace Utils
{
    inline auto db(double dt) -> double
    {
        std::random_device rd;  // Seed source
        unsigned int seed = rd();  // Seed source
        // unsigned int seed = 1;  // Seed source
        std::mt19937 generator(seed); // Mersenne Twister engine seeded with rd()
        double stdev = std::sqrt(dt);
        std::normal_distribution<> d(0.0, stdev);
        return d(generator);
    }

    inline auto BrownianMotion() -> SDE
    {
        auto drift = [](TIME, STATE){ return 0; };
        auto diffusion = [](TIME, STATE){ return 1; };
        return SDE(drift, diffusion);
    }
}