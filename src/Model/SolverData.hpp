#pragma once
#include "Types.hpp"
#include "DriftDiffusion.hpp"
#include <random>

using SolverFuncPtr = State (*)(const Drift&, const Diffusion&, Time,
    const State&, Time, std::mt19937&);

namespace dXt{
    inline auto dB(const Time dt, std::mt19937& gen) -> double {
        std::normal_distribution<double> dist(0.0, std::sqrt(dt));
        return dist(gen);
    }

    static auto EM =  [](const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, 
        std::mt19937& generator) -> State
    {
        return drift(t, Xt) * dt + diffusion(t, Xt) * dB(dt, generator);
    };

    static auto RK =  [](const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, 
        std::mt19937& generator) -> State
    {
        State Xt_hat = Xt + drift(t, Xt)*dt + diffusion(t, Xt)*std::sqrt(dt);
        double tmp_dB = dB(dt, generator);
        State RKCorrection = 0.5 * (diffusion(t, Xt_hat) - diffusion(t, Xt)) * (tmp_dB * tmp_dB - dt) * (1/std::sqrt(dt));
        return drift(t, Xt) * dt + diffusion(t, Xt) * tmp_dB + RKCorrection;
    };

}

inline auto dXtFunctor(SolverType solverType) -> SolverFuncPtr {
    switch (solverType) {
        case SolverType::EULER_MARUYAMA:
            return dXt::EM;
        case SolverType::RUNGE_KUTTA:
            return dXt::RK;
        default:
            throw std::runtime_error("No solver chosen...");
    };
}

// auto PathEngine::Increment(
//     const Drift& drift,
//     const Diffusion& diffusion,
//     const Time t,
//     const State Xt,
//     const Time dt,
//     std::mt19937& generator) const -> State
// {
//     const double dB = [&generator, dt](){
//         double stdev = std::sqrt(dt);
//         std::normal_distribution<> d(0.0, stdev);
//         return d(generator);
//     }();
//     return drift(t, Xt) * dt + diffusion(t, Xt) * dB;
// }