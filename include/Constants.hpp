#pragma once
#include "Types.hpp"
#include <numbers>
namespace DefaultConstants{
    constexpr ProcessType process = ProcessType::BM;
    constexpr double PI = std::numbers::pi_v<double>;
    constexpr size_t pdfPoints = static_cast<size_t>(2e3);
    constexpr size_t maxPathsToDraw = 20;
    constexpr size_t guiUpdateRate = 20; // milliseconds
    constexpr double OUthetaData = 0.5; // Mean-reversion coefficient for OU process (Hard coded for now...)
    namespace Simulation {
        constexpr SolverType solver = SolverType::EULER_MARUYAMA;
        constexpr Time time = 5;
        constexpr Time dt = 0.01;
        constexpr size_t samples = 1;
    }
}