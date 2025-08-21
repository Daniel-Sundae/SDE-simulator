#pragma once

#include <future>
#include <atomic>
#include <stop_token>
#include <memory>
#include "Types.hpp"

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
    {
    }
    const size_t totalPaths{};
    const Type type{};
    std::future<Paths> result{};
    size_t transactionNr{};
    std::shared_ptr<std::atomic<Job::Status>> status = std::make_shared<std::atomic<Job::Status>>(Job::Status::Pending);
    std::shared_ptr<std::atomic<size_t>> pathsCompleted = std::make_shared<std::atomic<size_t>>(0);
    std::stop_source stop{};
    void doCancel() { stop.request_stop(); }
};
