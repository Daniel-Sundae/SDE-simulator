#include "PathEngine.hpp"
#include "Utils.hpp"
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
    , m_isRunning(false)
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
        {
            std::scoped_lock sl(m_pathsMtx);
            // Check again before writing
            if (m_cancelRequested) return;
            m_paths[slot] = std::move(path);
        }
        m_completedTasks.fetch_add(1);
        m_completionCv.notify_one();
    };
}

auto PathEngine::SamplePathsAsync(const PathQuery& pathQuery, std::function<void(Paths)> onCompletionCb) -> void
{
    bool expect = false;
    if (!m_isRunning.compare_exchange_strong(expect, true)) {
        return;
    }
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
            m_tp->Enqueue(SamplePathGenerator(pathQuery, i));
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
        m_tp->ClearTasks();
        onCompletionCb(std::move(returnVal));
        m_isRunning = false;
    };
    m_tp->Enqueue(mainTask);
}

auto PathEngine::RequestCancel() -> void
{
    m_cancelRequested = true;
    m_completionCv.notify_one();
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