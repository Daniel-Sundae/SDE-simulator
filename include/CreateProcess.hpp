#pragma once

#include "Types.hpp"
#include "BM.hpp"
#include "GBM.hpp"

template<ProcessType Type>
struct Process;

template<>
struct Process<ProcessType::BM> {
	static auto Create(const ProcessDefinition& def) -> std::unique_ptr<BM>
	{
		auto drift = [](Time, State) { return 0.0; };
		auto diffusion = [](Time, State) { return 1.0; };
		return std::make_unique<BM>(drift, diffusion, def.startValue);
	}
};

template<>
struct Process<ProcessType::GBM> {
	static auto Create(const ProcessDefinition& def) -> std::unique_ptr<GBM>
	{
		auto drift = [def](Time, State s) { return def.mu * s; };
		auto diffusion = [def](Time, State s) { return def.sigma * s; };
		return std::make_unique<GBM>(drift, diffusion, def.startValue);
	}
};

//template<ProcessType Type>
//struct Process;
//
//template<>
//struct Process<ProcessType::BM> {
//	static auto Create(const double startPos) -> std::unique_ptr<BM>
//	{
//		auto drift = [](Time, State) { return 0.0; };
//		auto diffusion = [](Time, State) { return 1.0; };
//		return std::make_unique<BM>(drift, diffusion, startPos);
//	}
//};
//
//template<>
//struct Process<ProcessType::GBM> {
//	static auto Create(const double mu, const double sigma, const double startPos) -> std::unique_ptr<GBM>
//	{
//		auto drift = [mu](Time, State s) { return mu * s; };
//		auto diffusion = [sigma](Time, State s) { return sigma * s; };
//		return std::make_unique<GBM>(drift, diffusion, startPos);
//	}
//};





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