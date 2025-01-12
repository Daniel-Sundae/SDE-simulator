#pragma once
#include "Types.hpp"
#include "SDE.hpp"
class BM : public SDE {
	using BaseClass = SDE;
public:
	explicit BM(std::function<double(Time, State)>drift, std::function<double(Time, State)>diffusion, const double startPos);
public:
	auto Sample(int points, double dt) const -> Path override;
};