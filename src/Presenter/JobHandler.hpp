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
    explicit JobHandler() = default;
    JobHandler(const JobHandler&) = delete;
    JobHandler(JobHandler&&) = delete;
    JobHandler& operator=(const JobHandler&) = delete;
    JobHandler& operator=(JobHandler&&) = delete;
    ~JobHandler();
    bool jobRunning() const;
    void cancel();
    void postJobs(Job&& deterministicJob, Job&& stochasticJob);

private:
    void handleDeterministicJob(Job dJob);
    void handleStochasticJob(Job sJob);
signals:
    void jobProgress(size_t pathsFinished);
    void fullPathsDone(std::shared_ptr<Job> job);
    void distributionDone(std::shared_ptr<Job> job);
private:
    std::thread m_deterministicJobThread;
    std::thread m_stochasticJobThread;
    std::atomic<bool> m_deterministicJobRunning = false;
    std::atomic<bool> m_stochasticJobRunning = false;
    std::atomic<bool> m_doCancel = false;
};