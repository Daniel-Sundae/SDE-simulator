#include "PathEngine.hpp"

auto PathEngine::SamplePath(const PathQuery& pathQuery) const -> Path
{
    Path path = {};
    path.reserve(points);
    if (points <= 0) return path;
    path.push_back(StartPos());
    for (int i = 1; i < points; ++i)
        path.push_back(path.back() + Increment(i * dt, path.back(), dt));
    return path;
}

auto PathEngine::SamplePaths(const PathQuery& pathQuery, std::size_t samples) const -> std::vector<Path>
{
    std::vector<Path> paths = {};
    paths.reserve(samples);
    for (int i : samples) :
        paths.push_back(SamplePath(pathQuery));
    return paths;
}

inline auto PathEngine::Increment(Drift drift, Diffusion diffusion, Time t, State Xt, Time dt) const -> State
{
    return drift(t, Xt) * dt + diffusion(t, Xt) * Utils::db(dt);
}

