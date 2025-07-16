#pragma once
#include "Types.hpp"
#include "DriftDiffusion.hpp"
#include <random>


namespace dXt{
    inline auto dB(const Time dt, std::mt19937& gen) -> double {
        std::normal_distribution<double> dist(0.0, std::sqrt(dt));
        return dist(gen);
    }

    auto EM =  [](const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, 
        std::mt19937& generator) -> State
    {
        return drift(t, Xt) * dt + diffusion(t, Xt) * dB(dt, generator);
    };

    auto RK =  [](const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, 
        std::mt19937& generator) -> State
    {
        double tmp_dB = dB(dt, generator);
        double tmp_driftXt = drift(t, Xt); 
        double tmp_diffusionXt = diffusion(t, Xt); 
        State Xt_hat = Xt + tmp_driftXt * dt + tmp_diffusionXt * std::sqrt(dt);
        State RKCorrection = 0.5 * (diffusion(t, Xt_hat) - tmp_diffusionXt) * (tmp_dB * tmp_dB - dt) * (1/std::sqrt(dt));
        return tmp_driftXt * dt + tmp_diffusionXt * tmp_dB + RKCorrection;
    };

    auto MILSTEIN =  [](const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, 
        std::mt19937& generator) -> State
    {
        double tmp_dB = dB(dt, generator);
        double tmp_diffusionXt = diffusion(t, Xt); 
        State milsteinCorrection = 0.5 * tmp_diffusionXt * diffusion.prime(t, Xt) * (tmp_dB * tmp_dB - dt);
        return drift(t, Xt) * dt + tmp_diffusionXt * tmp_dB + milsteinCorrection;
    };

}

// Use function pointer instead of lambda for type matching
using SolverFuncPtr = State (*)(const Drift&, const Diffusion&, Time,
    const State&, Time, std::mt19937&);

inline auto dXtFunctor(SolverType solverType) -> SolverFuncPtr {
    switch (solverType) {
        case SolverType::EULER_MARUYAMA:
            return dXt::EM;
        case SolverType::RUNGE_KUTTA:
            return dXt::RK;
        case SolverType::MILSTEIN:
            return dXt::MILSTEIN;
        default:
            throw std::runtime_error("No solver chosen...");
    };
}
