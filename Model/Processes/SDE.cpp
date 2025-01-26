#include <vector>
#include "Process.hpp"
#include "Utils.hpp"

Process::Process(
    const std::function<double(Time, State)> drift,
    const std::function<double(Time, State)> diffusion,
    const double startPos
):
m_drift(std::move(drift)),
m_diffusion(std::move(diffusion)),
m_startPos(startPos)
{}

auto Process::Sample(int points, double dt) const -> Path
{
    Path path = {};
    path.reserve(points);
    if(points <= 0) return path;
    path.push_back(StartPos());
    for(int i=1; i<points; ++i)
    {
        path.push_back(path.back() + Increment(i * dt, path.back(), dt));
    }
    return path;
}

auto Process::Drift() const -> std::function<double(Time, State)>
{
    return m_drift;
}

auto Process::Diffusion() const -> std::function<double(Time, State)>
{
    return m_diffusion;
}

auto Process::StartPos() const -> double
{
    return m_startPos;
}

auto Process::Increment(double TIME, double STATE, double dt) const -> double
{
    return Drift()(TIME, STATE) * dt + Diffusion()(TIME, STATE) * Utils::db(dt);
}


