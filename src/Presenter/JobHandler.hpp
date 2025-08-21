#pragma once

#include "Types.hpp"
#include "Job.hpp"
#include <semaphore>
#include <thread>
#include <stop_token>
#include <atomic>
#include <memory>
#include <mutex>
#include <utility>
#include <QObject>

class JobHandler : public QObject{
    Q_OBJECT
public:
    explicit JobHandler();
    ~JobHandler();
    bool jobRunning() const;
    void cancel();
    void postJobs(std::pair<Job, Job>&& jobs);
private:
    void handleJobs(std::stop_token token);
signals:
    void jobProgress(size_t pathsFinished);
    void jobStatus(Job::Status status);
    void jobDone(std::shared_ptr<Job> job);
private:
    std::jthread m_jobConsumer;
    std::binary_semaphore m_jobsAvailable{0};
    std::atomic<bool> m_handlingJobs = false;
    std::atomic<bool> m_doCancel = false;
    std::optional<std::pair<Job, Job>> m_currentJobs;
};