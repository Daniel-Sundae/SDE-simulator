#include "PathEngine.hpp"
#include "Utils.hpp"
#include "PathQuery.hpp"
#include "PDFQuery.hpp"
#include <thread>
#include "assert.h"

auto PathEngine::SamplePaths(const PathQuery& pathQuery) const -> Paths
{
    const auto samplePath = [this, &pathQuery]() -> Path {
        const std::size_t points = pathQuery.simulationParameters.Points();
        const auto& drift = pathQuery.processDefinition.drift;
        const auto& diffusion = pathQuery.processDefinition.diffusion;
        const Time dt = pathQuery.simulationParameters.dt;
        const State startValueData = pathQuery.processDefinition.startValueData;
        Path path = {};
        assert(points != 0);
        path.reserve(points);
        path.push_back(startValueData);
        for (std::size_t i = 1; i < points; ++i)
            path.push_back(path.back() + Increment(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt));
        return path;
    };
    Paths paths;
    const std::size_t samples = pathQuery.simulationParameters.samples;
    paths.reserve(samples);
    std::vector<std::jthread> threads;
    threads.reserve(samples);
    for (std::size_t i = 0; i<samples; ++i)
        Path p = std::jthread(samplePath);
        paths.push_back(
    return paths;
}

auto PathEngine::GeneratePDFData(const PDFQuery& pdfQuery) const -> PDFData
{
    return pdfQuery.pdf.GeneratePDFData(pdfQuery.points);
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

