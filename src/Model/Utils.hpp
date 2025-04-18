#pragma once
#include <random>
#include <cmath>

namespace Utils
{
    inline auto db(double dt) -> double
    {
        auto threadLocalGenerator = []() -> std::mt19937&{
            // Each thread should have exactly one generator
            thread_local std::mt19937 generator(std::random_device{}());
            return generator;
        };
        double stdev = std::sqrt(dt);
        std::normal_distribution<> d(0.0, stdev);
        return d(threadLocalGenerator());
    }
}
