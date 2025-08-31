#include "JobHandler.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include <chrono>
#include <memory>

JobHandler::~JobHandler() {
    m_doCancel.store(true);
    if (m_deterministicJobThread.joinable()) m_deterministicJobThread.join();
    if (m_stochasticJobThread.joinable()) m_stochasticJobThread.join();
    if (m_stochasticFullPathsJobThread.joinable()) m_stochasticFullPathsJobThread.join();
}

void JobHandler::cancel() {
    m_doCancel.store(true);
}

bool JobHandler::jobRunning() const {
    return m_deterministicJobRunning.load()
        || m_stochasticJobRunning.load()
        || m_stochasticFullPathsJobRunning.load();
}

void JobHandler::postJobs(
        DeterministicJob&& deterministicJob,
        StochasticJob&& stochasticJob,
        StochasticFullPathsJob&& stochasticFullPathJob
    ) {
    Utils::assertTrue(!jobRunning(), "Cannot post jobs while another job is running");
    m_deterministicJobRunning.store(true);
    m_stochasticJobRunning.store(true);
    m_stochasticFullPathsJobRunning.store(true);
    m_doCancel.store(false);
    // Non blocking join due to check no job is running
    if (m_deterministicJobThread.joinable()) m_deterministicJobThread.join();
    if (m_stochasticJobThread.joinable()) m_stochasticJobThread.join();
    if (m_stochasticFullPathsJobThread.joinable()) m_stochasticFullPathsJobThread.join();
    m_deterministicJobThread = std::thread(&JobHandler::handleDeterministicJob, this, std::move(deterministicJob));
    m_stochasticJobThread = std::thread(&JobHandler::handleStochasticJob, this, std::move(stochasticJob));
    m_stochasticFullPathsJobThread = std::thread(&JobHandler::handleStochasticFullPathsJob, this, std::move(stochasticFullPathJob));
}

void JobHandler::handleDeterministicJob(DeterministicJob dJob) {
    while(dJob.drift.wait_for(std::chrono::milliseconds(DefaultConstants::guiUpdateRate)) != std::future_status::ready) {
        if (m_doCancel.load()) dJob.stop.request_stop();
    }
    emit driftDone(std::move(dJob.drift.get()),
        dJob.metaData->minXt.load(), dJob.metaData->maxXt.load());
    m_deterministicJobRunning.store(false);
}

void JobHandler::handleStochasticJob(StochasticJob sJob) {
    const auto emitData = [this, &sJob] {
        emit distributionJobData(
            sJob.metaData->pathsCompleted.load(),
            sJob.metaData->minXT.load(),
            sJob.metaData->maxXT.load(),
            sJob.metaData->minXt.load(),
            sJob.metaData->maxXt.load()
        );
    };
    while(sJob.distribution.wait_for(std::chrono::milliseconds(DefaultConstants::guiUpdateRate)) != std::future_status::ready) {
        if (m_doCancel.load()) sJob.stop.request_stop();
        emitData();
    }
    emitData();
    emit distributionDone(std::move(sJob.distribution.get()),
        sJob.metaData->minXT.load(), sJob.metaData->maxXT.load());
    m_stochasticJobRunning.store(false);
}

void JobHandler::handleStochasticFullPathsJob(StochasticFullPathsJob fpJob) {
    while(fpJob.fullPaths.wait_for(std::chrono::milliseconds(DefaultConstants::guiUpdateRate)) != std::future_status::ready) {
        if (m_doCancel.load()) fpJob.stop.request_stop();
    }
    emit fullPathsDone(std::move(fpJob.fullPaths.get()), fpJob.metaData->minXt.load(), fpJob.metaData->maxXt.load());
    m_stochasticFullPathsJobRunning.store(false);
}
