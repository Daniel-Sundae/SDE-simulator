#pragma once
#include "Types.hpp"
#include "SDE.hpp"

namespace SDEs{

    // dX = dB
    inline auto BrownianMotion() -> SDE
    {
        auto drift = [](TIME, STATE){ return 0; };
        auto diffusion = [](TIME, STATE){ return 1; };
        return SDE(drift, diffusion);
    }

    // dX = mue * X * dt + sigma * X * dB
    inline auto GeometricBrownianMotion(const double mue, const double sigma) -> SDE
    {
        auto drift = [mue](TIME, STATE s){ return mue * s; };
        auto diffusion = [sigma](TIME, STATE s){ return sigma * s; };
        return SDE(drift, diffusion);
    }
}