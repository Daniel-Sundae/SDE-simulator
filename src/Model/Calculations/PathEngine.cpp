#include "PathEngine.hpp"
#include "PathQuery.hpp"
#include "PDFQuery.hpp"
#include <thread>
#include <cassert>

PathEngine::PathEngine()
    : m_tp(std::make_unique<EngineThreadPool>())
    , m_paths()
    , m_pathsMtx()
    , m_cancelRequested(false)
    , m_completedTasks(0)
    , m_completionCv()
    , m_completionMtx()
    , m_isBusy(false)
    , m_engineSettings()
{ }

auto PathEngine::SamplePathGenerator(const PathQuery& pathQuery, const std::size_t slot) -> std::function<void()>
{
    return [this, pathQuery, slot]() -> void {
        const std::size_t points = pathQuery.simulationParameters.Points();
        const auto& drift = pathQuery.processDefinition.drift;
        const auto& diffusion = pathQuery.processDefinition.diffusion;
        const Time dt = pathQuery.simulationParameters.dt;
        const State startValueData = pathQuery.processDefinition.startValueData;
        Path path = {};
        assert(points != 0);
        path.reserve(points);
        path.push_back(startValueData);
        for (std::size_t i = 1; i < points; ++i) {
            if (i % 100 == 0 && m_cancelRequested.load()) {
                return;
            }
            path.push_back(path.back() + this->Increment(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt));
        }
        // Check again before writing
        if (m_cancelRequested) return;
        // Don't need to protect vector since each thread 
        // is given unique slot with preallocated space
        m_paths[slot] = std::move(path);
        
        // Important to lock to prevent mainTask reading stale data.
        // The unlocking operation guarantees the increment is visible to
        // mainTask thread before it evaluates predicate.
        {
            std::scoped_lock lock(m_completionMtx);
            m_completedTasks.fetch_add(1);
        }
        m_completionCv.notify_one();
    };
}

auto PathEngine::SampleDriftCurve(const PathQuery& pathQuery) const -> Path
{
    const std::size_t points = pathQuery.simulationParameters.Points();
    const auto& drift = pathQuery.processDefinition.drift;
    const auto& diffusion = pathQuery.processDefinition.diffusion;
    const Time dt = pathQuery.simulationParameters.dt;
    const State startValueData = pathQuery.processDefinition.startValueData;
    Path path = {};
    assert(points != 0);
    path.reserve(points);
    path.push_back(startValueData);
    for (std::size_t i = 1; i < points; ++i) {
        path.push_back(path.back() + Increment(drift, diffusion, static_cast<Time>(i) * dt, path.back(), dt));
    }
    return path;
}

auto PathEngine::SamplePathsAsync(const PathQuery& pathQuery, std::function<void(Paths)> onCompletionCb) -> void
{
    if(m_isBusy.load()){
        throw std::runtime_error("Engine is busy. Aborting transaction.");
    }
    m_isBusy = true;
    m_engineSettings = pathQuery.settingsParameters;

    auto mainTask = [this, pathQuery, onCompletionCb]() -> void{
        m_cancelRequested = false;
        const std::size_t nrSamples = pathQuery.simulationParameters.samples;
        {
            std::scoped_lock sl(m_pathsMtx);
            m_paths.clear();
            m_paths.resize(nrSamples);
        }
        m_completedTasks = 0;
        for (std::size_t i = 0; i < nrSamples; ++i) {
            m_paths[i].reserve(pathQuery.simulationParameters.Points());
            m_engineSettings.useThreading ? m_tp->Enqueue(SamplePathGenerator(pathQuery, i)) : SamplePathGenerator(pathQuery, i)();
        }

        // Wait until tasks done or GUI thread cancels
        {
            std::unique_lock<std::mutex> lock(m_completionMtx);
            m_completionCv.wait(lock, [this, nrSamples]() {return m_completedTasks == nrSamples || m_cancelRequested;});
        }
        Paths returnVal;
        {
            std::scoped_lock sl(m_pathsMtx);
            returnVal = m_cancelRequested.load() ? Paths{} : std::move(m_paths);
            m_paths.clear();
        }
        m_tp->ClearTasks(); // Flush queue incase cancel requested
        onCompletionCb(std::move(returnVal));
        m_isBusy = false;
    };
    m_tp->Enqueue(mainTask);
}

auto PathEngine::RequestCancel() -> void
{
    m_cancelRequested = true;
    m_completionCv.notify_one();
}

auto PathEngine::IsBusy() -> bool
{
    return m_isBusy.load() ? true : false;
}

auto PathEngine::Increment(
    const Drift& drift,
    const Diffusion& diffusion,
    const Time t,
    const State Xt,
    const Time dt) const -> State
{
    return drift(t, Xt) * dt + diffusion(t, Xt) * db(dt);
}

auto PathEngine::db(double dt) const -> double
{
    // Each thread should have exactly one generator
    thread_local std::mt19937 generator(m_engineSettings.useSeed.first ? m_engineSettings.useSeed.second : std::random_device{}());
    double stdev = std::sqrt(dt);
    std::normal_distribution<> d(0.0, stdev);
    return d(generator);
}