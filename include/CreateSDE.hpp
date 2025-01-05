#pragma once
#include "Types.hpp"
#include "SDE.hpp"

namespace CreateSDE{

    // dX = dB
    inline auto BrownianMotion(const double startPos = 0) -> SDE
    {
        auto drift = [](TIME, STATE){ return 0; };
        auto diffusion = [](TIME, STATE){ return 1; };
        return SDE(drift, diffusion, startPos);
    }

    // dX = μXdt + σXdB
    inline auto GeometricBrownianMotion(const double mue, const double sigma, const double startPos) -> SDE
    {
        auto drift = [mue](TIME, STATE s){ return mue * s; };
        auto diffusion = [sigma](TIME, STATE s){ return sigma * s; };
        return SDE(drift, diffusion, startPos);
    }
}