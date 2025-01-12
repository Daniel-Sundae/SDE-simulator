#pragma once

#include <functional>
#include <vector>
#include "Types.hpp"

class SDE{
public:
    SDE() = delete;
    explicit SDE(std::function<double(Time, State)>drift, std::function<double(Time, State)>diffusion, const double startPos);
    virtual ~SDE() = default;
public:
    virtual auto Sample(int points, double dt) const -> Path = 0;
    auto Drift() const -> std::function<double(Time, State)>;
    auto Diffusion() const -> std::function<double(Time, State)>;
    auto StartPos() const -> double;

private:
    auto Increment(double TIME, double STATE, double dt) const -> double;
private:
    const std::function<double(Time, State)> m_drift;
    const std::function<double(Time, State)> m_diffusion;
    const double m_startPos;
};
