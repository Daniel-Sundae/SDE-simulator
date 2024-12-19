#pragma once

#include <functional>
#include <vector>

using time = double;
using state = double;

class SDE{
public:
    SDE(std::function<double(time, state)>drift, std::function<double(time, state)>diffusion);
    auto SamplePath(int points, double dt, double startPos = 0) const -> const std::vector<double>;
    auto Drift() const -> std::function<double(time, state)>;
    auto Diffusion() const -> std::function<double(time, state)>;

private:
    auto Increment(double time, double state, double dt) const -> double;

    const std::function<double(time, state)> m_drift;
    const std::function<double(time, state)> m_diffusion;
};
