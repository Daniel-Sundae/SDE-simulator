#pragma once
#include "Types.hpp"
#include "BM.hpp"
#include "GBM.hpp"

template<SDEType Type>
struct SDE;
	
template<>
struct SDE<SDEType::BM> {
	static auto Create(const double startPos) -> std::unique_ptr<BM> 
	{
	    auto drift = [](Time, State) { return 0.0; };
	    auto diffusion = [](Time, State) { return 1.0; };
	    return std::make_unique<BM>(drift, diffusion, startPos);
	}
};
	
template<>
struct SDE<SDEType::GBM> {
	static auto Create(const double mu, const double sigma, const double startPos) -> std::unique_ptr<GBM>
	{
	    auto drift = [mu](Time, State s) { return mu * s; };
	    auto diffusion = [sigma](Time, State s) { return sigma * s; };
	    return std::make_unique<GBM>(drift, diffusion, startPos);
	}
};






//// dX = dB
//inline auto BrownianMotion(const double startPos) -> std::unique_ptr<BM>
//{
//    auto drift = [](Time, State){ return 0; };
//    auto diffusion = [](Time, State){ return 1; };
//    return BM(drift, diffusion, startPos);
//}

//// dX = μXdt + σXdB
//inline auto GeometricBrownianMotion(const double mu, const double sigma, const double startPos) -> GBM
//{
//    auto drift = [mu](Time, State s){ return mu * s; };
//    auto diffusion = [sigma](Time, State s){ return sigma * s; };
//    return GBM(drift, diffusion, startPos);
//}