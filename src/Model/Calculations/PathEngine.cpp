#include "PathEngine.hpp"
#include "Utils.hpp"
#include "PathQuery.hpp"
#include "PDFQuery.hpp"
#include <thread>
#include <future>
#include "assert.h"

PathEngine::PathEngine()
    : m_tp(std::make_unique<EngineThreadPool>())
{ }

auto PathEngine::SamplePaths(const PathQuery& pathQuery) const -> Paths
{
    auto samplePath = [this, pathQuery]() -> Path {
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
            path.push_back(path.back() + this->Increment(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt));
        return path;
    };

    Paths paths;
    std::vector< std::future<Path> > futures;
    const std::size_t nrSamples = pathQuery.simulationParameters.samples;
    paths.reserve(nrSamples);
    futures.reserve(nrSamples);
    for (std::size_t i = 0; i < nrSamples; ++i) {
        futures.emplace_back(m_tp->Enqueue(samplePath, Priority::LOW));
    }
    for (auto& future : futures) {
        paths.push_back(future.get());
    }
    return paths;
}


auto PathEngine::SamplePath(const PathQuery& pathQuery) const -> Path
{
    (void)pathQuery;
    return {};
    //const std::size_t points = pathQuery.simulationParameters.Points();
    //const auto& drift = pathQuery.processDefinition.drift;
    //const auto& diffusion = pathQuery.processDefinition.diffusion;
    //const Time dt = pathQuery.simulationParameters.dt;
    //const State startValueData = pathQuery.processDefinition.startValueData;
    //Path path = {};
    //assert(points != 0);
    //path.reserve(points);
    //path.push_back(startValueData);
    //for (std::size_t i = 1; i < points; ++i)
    //    path.push_back(path.back() + Increment(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt));
    //return path;
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
    int dummy;
    (void)dummy;
}