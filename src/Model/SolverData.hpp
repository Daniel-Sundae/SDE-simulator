#pragma once
#include "Types.hpp"
#include "DriftDiffusion.hpp"
#include <random>


namespace dXt{
    inline double dB(const Time dt, std::mt19937& gen) {
        static thread_local std::normal_distribution<double> dist(0.0, 1.0);
        return std::sqrt(dt) * dist(gen);
    }

    State EM(const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, 
        std::mt19937& generator)
    {
        return drift(t, Xt) * dt + diffusion(t, Xt) * dB(dt, generator);
    }

    State RK(const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, 
        std::mt19937& generator)
    {
        double tmp_dB = dB(dt, generator);
        double tmp_driftXt = drift(t, Xt); 
        double tmp_diffusionXt = diffusion(t, Xt); 
        State Xt_hat = Xt + tmp_driftXt * dt + tmp_diffusionXt * std::sqrt(dt);
        State RKCorrection = 0.5 * (diffusion(t, Xt_hat) - tmp_diffusionXt) * (tmp_dB * tmp_dB - dt) * (1/std::sqrt(dt));
        return tmp_driftXt * dt + tmp_diffusionXt * tmp_dB + RKCorrection;
    }

    State MILSTEIN(const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, 
        std::mt19937& generator)
    {
        double tmp_dB = dB(dt, generator);
        double tmp_diffusionXt = diffusion(t, Xt); 
        State milsteinCorrection = 0.5 * tmp_diffusionXt * diffusion.prime(t, Xt) * (tmp_dB * tmp_dB - dt);
        return drift(t, Xt) * dt + tmp_diffusionXt * tmp_dB + milsteinCorrection;
    }
}

using SolverFuncPtr = State (*)(const Drift&, const Diffusion&, Time,
    const State&, Time, std::mt19937&);

inline auto dXtFunction(SolverType solverType) -> SolverFuncPtr {
    switch (solverType) {
        case SolverType::EULER_MARUYAMA:
            return dXt::EM;
        case SolverType::RUNGE_KUTTA:
            return dXt::RK;
        case SolverType::MILSTEIN:
            return dXt::MILSTEIN;
        default:
            Utils::fatalError("Solver {} not implemented", static_cast<int>(solverType));
    };
}
