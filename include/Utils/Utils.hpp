#pragma once
#include <random>
#include <cmath>
#include <QtCharts/QLineSeries>
#include "SDE.hpp"

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

    inline auto LineSeries(const std::vector<double>& data) -> std::unique_ptr<QLineSeries>
    {
        auto series = std::make_unique<QLineSeries>();
        for (size_t i = 0; i < data.size(); ++i) {
            series->append(static_cast<qreal>(i), static_cast<qreal>(data[i]));
        }
        return series;
    }
}