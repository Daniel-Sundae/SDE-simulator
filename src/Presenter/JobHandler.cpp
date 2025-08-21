#include "JobHandler.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include <chrono>
#include <memory>

JobHandler::JobHandler()
    : m_jobConsumer([this](std::stop_token token) {handleJobs(token);})
{
}

JobHandler::~JobHandler() {
    m_jobConsumer.request_stop(); // Request stop to the worker thread
    m_jobsAvailable.release(); // Ensure worker wakes up
    m_jobConsumer.join();
}

bool JobHandler::jobRunning() const {
    return m_handlingJobs.load();
}

void JobHandler::cancel() {
    m_doCancel.store(true);
}

void JobHandler::postJobs(std::pair<Job, Job>&& jobs) {
    if(m_handlingJobs.load()){
        Utils::fatalError("Job is already running. Should not be possible to post new jobs");
        return;
    }
    m_currentJobs.emplace(std::move(jobs));
    m_doCancel.store(false);
    m_jobsAvailable.release();
}

void JobHandler::handleJobs(std::stop_token token) {
    while (true) {
        m_jobsAvailable.acquire();
        if (token.stop_requested()){
            return;
        }
        m_handlingJobs.store(true);
        Job& deterministicJob = m_currentJobs->first;
        Job& stochasticJob = m_currentJobs->second;
        while (true) {
            if(m_doCancel.load()){
                deterministicJob.doCancel();
                stochasticJob.doCancel();
            }
            const bool dDone = deterministicJob.result.wait_for(std::chrono::milliseconds(DefaultConstants::guiUpdateRate)) == std::future_status::ready;
            const bool sDone = stochasticJob.result.wait_for(std::chrono::milliseconds(DefaultConstants::guiUpdateRate)) == std::future_status::ready;
            if (dDone && sDone) break;
            if (!sDone) {
                emit jobProgress(stochasticJob.pathsCompleted->load());
                emit jobStatus(stochasticJob.status->load());
            }
        }
        emit jobProgress(stochasticJob.pathsCompleted->load());
        emit jobStatus(stochasticJob.status->load());
        emit jobDone(std::make_shared<Job>(std::move(deterministicJob)));
        emit jobDone(std::make_shared<Job>(std::move(stochasticJob)));
        m_currentJobs = std::nullopt;
        m_handlingJobs.store(false);
    }
}