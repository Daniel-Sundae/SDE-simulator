#pragma once

#include "Types.hpp"
#include "Job.hpp"
#include <semaphore>
#include <thread>
#include <stop_token>
#include <atomic>
#include <memory>
#include <mutex>
#include <QObject>

class JobHandler : public QObject{
    Q_OBJECT
public:
    explicit JobHandler();
    ~JobHandler();
    bool jobRunning() const;
    void cancel();
    void postJob(Job &&job);
private:
    void handleJobs(std::stop_token token);
signals:
    void jobProgress(size_t pathsFinished, size_t totalPaths);
    void jobStatus(Job::Status status);
    void jobDone(Paths paths, Job::Type type, size_t jobNr);
private:
    std::jthread m_jobConsumer;
    std::binary_semaphore m_jobAvailable{0};
    std::counting_semaphore<1> m_slotsAvailable{1}; // Make 2 later
    std::mutex m_jobMtx;
    std::atomic<std::shared_ptr<Job>> m_currentJob = nullptr;
};