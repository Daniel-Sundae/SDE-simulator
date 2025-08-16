#pragma once
#include "Types.hpp"
#include "EngineThreadPool.hpp"
#include <memory>
#include <mutex>
#include <optional>
#include <functional>
#include <random>
#include <array>
#include <cmath>

struct PathQuery;
struct Transaction;
struct Job {
    enum class Type {
        Deterministic,
        Stochastic,
    };
    enum class Status {
        Pending,
        Running,
        Completed,
        Cancelled
    };
    explicit Job(size_t _totalPaths, Type _type)
    : totalPaths(_totalPaths)
    , type(_type)
    {}
    const size_t totalPaths{};
    const Type type{};
private:
    std::future<Paths> result;
public:
    // Shared pointer allows modification via copy
    std::shared_ptr<std::atomic<Job::Status>> status = std::make_shared<std::atomic<Job::Status>>(Job::Status::Pending);
    std::shared_ptr<std::atomic<size_t>> pathsCompleted = std::make_shared<std::atomic<size_t>>(0);
    std::stop_source stop{};
    bool isCancelled() const { return stop.stop_requested(); }
    bool doCancel()  { return stop.request_stop(); }
    bool isDone() const { return pathsCompleted->load() == totalPaths; }
    Paths getResult() { return std::move(result.get()); }
    void setResult(std::future<Paths> newResult) { result = std::move(newResult); }
};
class PathEngine{
public:
    explicit PathEngine() = default;
    [[nodiscard]] std::array<Job, Transaction::numQueries> processTransaction(const Transaction& transaction);
    void cancelJobs();
    bool isBusy();

private:
    State increment(const Drift &drift,
        const Diffusion &diffusion,
        const Time t,
        const State Xt,
        const Time dt,
        std::mt19937 &generator) const;

private:
    std::unique_ptr<EngineThreadPool> m_tp =
        std::make_unique<EngineThreadPool>();
};
