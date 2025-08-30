#pragma once
#include "Types.hpp"
#include "DriftDiffusion.hpp"
#include <random>


namespace dXt{
    State EM(const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, const double dB)
    {
        return drift(t, Xt) * dt + diffusion(t, Xt) * dB;
    }

    State RK(const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, const double dB)
    {
        double tmp_driftXt = drift(t, Xt); 
        double tmp_diffusionXt = diffusion(t, Xt); 
        State Xt_hat = Xt + tmp_driftXt * dt + tmp_diffusionXt * std::sqrt(dt);
        State RKCorrection = 0.5 * (diffusion(t, Xt_hat) - tmp_diffusionXt) * (dB * dB - dt) * (1/std::sqrt(dt));
        return tmp_driftXt * dt + tmp_diffusionXt * dB + RKCorrection;
    }

    State MILSTEIN(const Drift& drift, const Diffusion& diffusion, 
        const Time t, const State& Xt, const Time dt, const double dB)
    {
        double tmp_diffusionXt = diffusion(t, Xt); 
        State milsteinCorrection = 0.5 * tmp_diffusionXt * diffusion.prime(t, Xt) * (dB * dB - dt);
        return drift(t, Xt) * dt + tmp_diffusionXt * dB + milsteinCorrection;
    }
}

using SolverFuncPtr = State (*)(const Drift&, const Diffusion&, Time,
    const State&, const Time, const double);

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
