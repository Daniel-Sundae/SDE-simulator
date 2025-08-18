#include "JobHandler.hpp"
#include "Constants.hpp"
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
        while (!job->isDone()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DefaultConstants::loadingbarUpdateRate));
            emit jobProgress(job->pathsCompleted->load(), job->totalPaths);
            emit jobStatus(job->status->load());
        }
        emit jobProgress(job->pathsCompleted->load(), job->totalPaths);
        emit jobStatus(job->status->load());
        emit jobDone(job->get(), job->type);
        job.reset();
        m_currentJob.store(nullptr);
        m_slotsAvailable.release();
    }
}