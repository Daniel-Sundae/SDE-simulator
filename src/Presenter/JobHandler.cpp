#include "JobHandler.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include <chrono>


JobHandler::JobHandler()
    : m_jobConsumer([this](std::stop_token token) {handleJobs(token);})
{
}

JobHandler::~JobHandler() {
    m_jobConsumer.request_stop(); // Request stop to the worker thread
    m_jobAvailable.release(); // Ensure worker wakes up
    m_jobConsumer.join();
}

bool JobHandler::jobRunning() const {
    return m_currentJob.load() != nullptr;
}

void JobHandler::cancel() {
    // Copy shared pointer for thread safety
    auto job = m_currentJob.load();
    if (job) {
        job->doCancel();
    }
}

void JobHandler::postJob(Job&& job){
    if(!m_slotsAvailable.try_acquire()){
        return;
    }
    m_currentJob.store(std::make_shared<Job>(std::move(job)));
    m_jobAvailable.release();
}

void JobHandler::handleJobs(std::stop_token token) {
    while (true) {
        m_jobAvailable.acquire();
        if (token.stop_requested()){
            return;
        }
        std::shared_ptr<Job> job = m_currentJob.load();
        while (job->result.wait_for(std::chrono::milliseconds(DefaultConstants::loadingbarUpdateRate)) != std::future_status::ready) {
            emit jobProgress(job->pathsCompleted->load(), job->totalPaths);
            emit jobStatus(job->status->load());
        }
        size_t pathsCompleted = job->pathsCompleted->load();
        size_t totalPaths = job->totalPaths;
        if(pathsCompleted != totalPaths){
            Utils::fatalError("Expected completed paths: {} to equal total paths: {}", pathsCompleted, totalPaths);
        }
        emit jobProgress(pathsCompleted, totalPaths);
        emit jobStatus(job->status->load());
        emit jobDone(job->result.get(), job->type, job->jobNr);
        m_currentJob.store(nullptr);
        m_slotsAvailable.release();
    } // Job freed here
}