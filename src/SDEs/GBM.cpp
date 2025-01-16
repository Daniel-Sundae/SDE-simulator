#include "GBM.hpp"
#include <stdexcept>
#include <sstream>

GBM::GBM(
	std::function<double(Time, State)> drift,
	std::function<double(Time, State)> diffusion,
	const double startPos)
: BaseClass(std::move(drift), std::move(diffusion), startPos)
{}

auto GBM::Sample(int points, double dt) const -> Path
{
	if (StartPos() == 0){
		return std::vector<double>(points, 0);
	}
	else if (StartPos() < 0){
		std::stringstream ss;
		ss  << "Geometric brownian motion requires non-negative start position. "
			<< "Current startposition: " << StartPos();
		throw std::out_of_range(ss.str());
	}
	return BaseClass::Sample(points, dt);
}

