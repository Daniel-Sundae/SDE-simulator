#pragma once

#include <future>
#include <atomic>
#include <stop_token>
#include <memory>
#include <numeric>
#include "Types.hpp"

struct Job {
    enum class Type {
        Deterministic,
        Stochastic,
    };
    struct Atomics {
        std::atomic<size_t> pathsCompleted;
        std::atomic<State> minPathEndVal;
        std::atomic<State> maxPathEndVal;
    };
    explicit Job(size_t _totalPaths)
    : totalPaths(_totalPaths)
    {
    }
    const size_t totalPaths{};
    Type type{};
    std::future<Paths> fullPaths{};
    std::future<Distribution> distribution{};
    std::shared_ptr<Job::Atomics> atomicData =
        std::make_shared<Job::Atomics>(
            0,
            std::numeric_limits<State>::max(),
            std::numeric_limits<State>::min());
    std::stop_source stop{};
};
