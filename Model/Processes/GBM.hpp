#pragma once
#include "Process.hpp"
class GBM : public Process {
	using BaseClass = Process;
public:
	explicit GBM(std::function<double(Time, State)>drift, std::function<double(Time, State)>diffusion, const double startPos);
public:
	auto Sample(int points, double dt) const -> Path override;
};