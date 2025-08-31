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
    void postJobs(
        DeterministicJob&& dJob,
        StochasticJob&& sJob,
        StochasticFullPathsJob&& fpJob
    );
private:
    void handleDeterministicJob(DeterministicJob dJob);
    void handleStochasticJob(StochasticJob sJob);
    void handleStochasticFullPathsJob(StochasticFullPathsJob fpJob);
signals:
    void distributionJobData(size_t pathsFinished, State minXT, State maxXT, State minXt, State maxXt);
    void driftDone(Path drift, State minXt, State maxXt);
    void fullPathsDone(Paths paths, State minXt, State maxXt);
    void distributionDone(Distribution distribution, State minXT, State maxXT);
private:
    std::thread m_deterministicJobThread;
    std::thread m_stochasticJobThread;
    std::thread m_stochasticFullPathsJobThread;
    std::atomic<bool> m_deterministicJobRunning = false;
    std::atomic<bool> m_stochasticJobRunning = false;
    std::atomic<bool> m_stochasticFullPathsJobRunning = false;
    std::atomic<bool> m_doCancel = false;
};