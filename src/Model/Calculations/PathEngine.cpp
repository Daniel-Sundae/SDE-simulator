#include "PathEngine.hpp"
#include "Utils.hpp"

auto PathEngine::SamplePath(const PathQuery& pathQuery) const -> Path
{
    const std::size_t points = pathQuery.simulationParameters.points;
    auto drift = pathQuery.processDefinition.drift;
    auto diffusion = pathQuery.processDefinition.diffusion;
    const Time dt = pathQuery.simulationParameters.dt();
    const State startValue = pathQuery.processDefinition.startValue;
    Path path = {};
    path.reserve(points);
    if (points <= 0) return path;
    path.push_back(startValue);
    for (std::size_t i = 1; i < points; ++i)
        path.push_back(path.back() + Increment(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt));
    return path;
}

auto PathEngine::SamplePaths(const PathQuery& pathQuery, std::size_t samples) const -> std::vector<Path>
{
    std::vector<Path> paths = {};
    paths.reserve(samples);
    for (std::size_t i = 0; i<samples; ++i)
        paths.push_back(SamplePath(pathQuery));
    return paths;
}

auto PathEngine::Increment(
    const Drift& drift,
    const Diffusion& diffusion,
    const Time t,
    const State Xt,
    const Time dt) const -> State
{
    return drift(t, Xt) * dt + diffusion(t, Xt) * Utils::db(dt);
}

