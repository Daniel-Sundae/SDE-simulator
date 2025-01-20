#pragma once
#include "SDE.hpp"
class GBM : public SDE {
	using BaseClass = SDE;
public:
	explicit GBM(std::function<double(Time, State)>drift, std::function<double(Time, State)>diffusion, const double startPos);
public:
	auto Sample(int points, double dt) const -> Path override;
};