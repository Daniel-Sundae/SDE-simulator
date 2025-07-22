#pragma once
#include "Types.hpp"
#include <numbers>
namespace DefaultConstants{
    constexpr ProcessType process = ProcessType::BM;
    constexpr double PI = std::numbers::pi_v<double>;
    constexpr double pdfThreshold = 1e-6;
    constexpr size_t pdfPoints = static_cast<size_t>(1e3);
    constexpr size_t maxPathsToDraw = 20;
    constexpr double OUthetaData = 1.0; // Mean-reversion coefficient for OU process (Hard coded for now...)
    namespace Simulation {
        constexpr SolverType solver = SolverType::EULER_MARUYAMA;
        constexpr Time time = 10;
        constexpr Time dt = 0.01;
        constexpr size_t samples = 1;
    }
}