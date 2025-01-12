#pragma once
#include "Types.hpp"
#include "BM.hpp"
#include "GBM.hpp"

namespace CreateSDE{

    // dX = dB
    inline auto BrownianMotion(const double startPos = 0) -> BM
    {
        auto drift = [](Time, State){ return 0; };
        auto diffusion = [](Time, State){ return 1; };
        return BM(drift, diffusion, startPos);
    }

    // dX = μXdt + σXdB
    inline auto GeometricBrownianMotion(const double mue, const double sigma, const double startPos) -> GBM
    {
        auto drift = [mue](Time, State s){ return mue * s; };
        auto diffusion = [sigma](Time, State s){ return sigma * s; };
        return GBM(drift, diffusion, startPos);
    }
}