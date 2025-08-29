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
    auto job = std::make_shared<Job>(std::move(sJob));
    bool distributionEmitted = false;
    bool fullPathsEmitted = false;
    while(true) {
        if (m_doCancel.load()) job->stop.request_stop();
        if(!fullPathsEmitted && job->fullPaths.wait_for(std::chrono::milliseconds(DefaultConstants::guiUpdateRate)) == std::future_status::ready){
            emit fullPathsDone(job);
            fullPathsEmitted = true;
        }
        if(!distributionEmitted && job->distribution.wait_for(std::chrono::milliseconds(DefaultConstants::guiUpdateRate)) == std::future_status::ready) {
            emit distributionDone(job);
            distributionEmitted = true;
        }
        emit jobMetaData(
            job->metaData->pathsCompleted.load(),
            job->metaData->minXT.load(),
            job->metaData->maxXT.load(),
            job->metaData->minXt.load(),
            job->metaData->maxXt.load()
        );
        if (fullPathsEmitted && distributionEmitted) break;
    }
    m_stochasticJobRunning.store(false);
}
