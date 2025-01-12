#include "BM.hpp"

BM::BM(
	std::function<double(Time, State)> drift,
	std::function<double(Time, State)> diffusion,
	const double startPos)
	: BaseClass(std::move(drift), std::move(diffusion), startPos)
{}
//---------------------------------------------------------------------------//
auto BM::Sample(int points, double dt) const -> Path
{
	return BaseClass::Sample(points, dt);
}

