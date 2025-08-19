#pragma once

#include <future>
#include <atomic>
#include <stop_token>
#include <memory>
#include "Types.hpp"

struct Job {
    static inline std::atomic<size_t> s_deterministicJobCounter{0};
    static inline std::atomic<size_t> s_stochasticJobCounter{0};
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
    explicit Job(size_t _totalPaths, Type _type, std::future<Paths>&& _result)
    : totalPaths(_totalPaths)
    , type(_type)
    , result(std::move(_result))
    , jobNr(_type == Type::Deterministic
        ? ++s_deterministicJobCounter
        : ++s_stochasticJobCounter)
    {
    }
    const size_t totalPaths{};
    const Type type{};
    std::future<Paths> result{};
    const size_t jobNr{};
    // Shared pointer allows modification via copy
    std::shared_ptr<std::atomic<Job::Status>> status = std::make_shared<std::atomic<Job::Status>>(Job::Status::Pending);
    std::shared_ptr<std::atomic<size_t>> pathsCompleted = std::make_shared<std::atomic<size_t>>(0);
    std::stop_source stop{};
    void doCancel() { stop.request_stop(); }
};
