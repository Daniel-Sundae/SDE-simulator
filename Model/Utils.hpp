#pragma once
#include <random>
#include <cmath>

namespace Utils
{
    inline auto db(double dt) -> double
    {
        std::random_device rd;
        unsigned int seed = rd();  // Seed source
        // unsigned int seed = 1;  // Deterministic source
        std::mt19937 generator(seed);
        double stdev = std::sqrt(dt);
        std::normal_distribution<> d(0.0, stdev);
        return d(generator);
    }

}