#pragma once
#include <random>
#include <cmath>

namespace Utils
{
    // Static generator to avoid recreating for each call
    namespace {
        std::mt19937 generator(std::random_device{}());
    }
    
    inline auto db(double dt) -> double
    {
        double stdev = std::sqrt(dt);
        std::normal_distribution<> d(0.0, stdev);
        return d(generator);
    }
}
