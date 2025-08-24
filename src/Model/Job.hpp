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
    struct Atomics {
        std::atomic<size_t> pathsCompleted;
        // State maxPathEndVal;
    };
    explicit Job(size_t _totalPaths)
    : totalPaths(_totalPaths)
    {
    }
    const size_t totalPaths{};
    Type type{};
    std::future<Paths> fullPaths{};
    std::future<Distribution> distribution{};
    size_t transactionNr{};
    std::shared_ptr<Job::Atomics> atomicData =
        std::make_shared<Job::Atomics>();
    std::stop_source stop{};
};
