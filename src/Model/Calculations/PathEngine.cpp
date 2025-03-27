#include "PathEngine.hpp"
#include "Utils.hpp"
#include "PathQuery.hpp"
#include "PDFQuery.hpp"
#include <thread>
#include "assert.h"

PathEngine::PathEngine(std::unique_ptr<EngineThreadPool> tp)
    : std::move(tp)
    , m_isBusy(false)
{ }

auto PathEngine::SamplePaths(const PathQuery& pathQuery) const -> Paths
{
    Paths paths;
    const std::size_t samples = pathQuery.simulationParameters.samples;
    paths.reserve(samples);
    for (std::size_t i = 0; i < samples; ++i)
        paths.push_back(SamplePath(pathQuery));
    return paths;
}

auto PathEngine::SamplePath(const PathQuery& pathQuery) const -> Path {
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

auto PathEngine::Stop() const -> void
{
    if (m_isBusy) { // Probably do not need this variable
        // Stop threadpool
    }
}