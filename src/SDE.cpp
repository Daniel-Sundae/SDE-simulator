#include <vector>
#include "SDE.hpp"
#include "Util.hpp"

SDE::SDE(
    const std::function<double(time, state)> drift,
    const std::function<double(time, state)> diffusion
):
m_drift(std::move(drift)),
m_diffusion(std::move(diffusion))
{}
//---------------------------------------------------------------------------//
auto SDE::SamplePath(int points, double dt, double startPos) const -> const std::vector<double>
{
    std::vector<double> path;
    path.reserve(points);
    if(points <= 0) return path;
    path[0] = startPos;
    for(int i=1; i<points; ++i)
    {
        path.push_back(path.back() + Increment(i * dt, path.back(), dt));
    }
    return path;
}
//---------------------------------------------------------------------------//
auto SDE::Drift() const -> std::function<double(time, state)>
{
    return m_drift;
}
//---------------------------------------------------------------------------//
auto SDE::Diffusion() const -> std::function<double(time, state)>
{
    return m_diffusion;
}
//---------------------------------------------------------------------------//
auto SDE::Increment(double time, double state, double dt) const -> double
{
    return Drift()(time, state) * dt + Diffusion()(time, state) * Util::db(dt);
}
//---------------------------------------------------------------------------//

