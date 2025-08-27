#include "JobHandler.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include <chrono>
#include <memory>

JobHandler::~JobHandler() {
    m_doCancel.store(true);
    if (m_deterministicJobThread.joinable()) m_deterministicJobThread.join();
    if (m_stochasticJobThread.joinable()) m_stochasticJobThread.join();
}

void JobHandler::cancel() {
    m_doCancel.store(true);
}

bool JobHandler::jobRunning() const {
    return m_deterministicJobRunning.load() || m_stochasticJobRunning.load();
}

void JobHandler::postJobs(Job&& deterministicJob, Job&& stochasticJob) {
    Utils::assertTrue(!jobRunning(), "Cannot post jobs while another job is running");
    m_deterministicJobRunning.store(true);
    m_stochasticJobRunning.store(true);
    m_doCancel.store(false);
    // Non blocking join due to check no job is running
    if (m_deterministicJobThread.joinable()) m_deterministicJobThread.join();
    if (m_stochasticJobThread.joinable()) m_stochasticJobThread.join();
    m_deterministicJobThread = std::thread(&JobHandler::handleDeterministicJob, this, std::move(deterministicJob));
    m_stochasticJobThread = std::thread(&JobHandler::handleStochasticJob, this, std::move(stochasticJob));
}

void JobHandler::handleDeterministicJob(Job dJob) {
    while(dJob.distribution.wait_for(std::chrono::milliseconds(DefaultConstants::guiUpdateRate)) != std::future_status::ready) {
        if (m_doCancel.load()){
            dJob.stop.request_stop();
        }
    }
    emit fullPathsDone(std::make_shared<Job>(std::move(dJob)));
    m_deterministicJobRunning.store(false);
}

void JobHandler::handleStochasticJob(Job sJob) {
    while(sJob.distribution.wait_for(std::chrono::milliseconds(DefaultConstants::guiUpdateRate)) != std::future_status::ready
            || sJob.fullPaths.wait_for(std::chrono::milliseconds(DefaultConstants::guiUpdateRate)) != std::future_status::ready) {
        if (m_doCancel.load()){
            sJob.stop.request_stop();
        }
        emit jobProgress(sJob.atomicData->pathsCompleted.load());
    }
    emit jobProgress(sJob.atomicData->pathsCompleted.load());
    auto job = std::make_shared<Job>(std::move(sJob));
    emit fullPathsDone(job);
    emit distributionDone(job);
    m_stochasticJobRunning.store(false);
}
