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
    struct MetaData {
        std::atomic<size_t> pathsCompleted;
        std::atomic<State> minXT;
        std::atomic<State> maxXT;
        std::atomic<State> minXt;
        std::atomic<State> maxXt;
    };
    explicit Job(size_t _totalPaths)
    : totalPaths(_totalPaths)
    {
    }
    const size_t totalPaths{};
    Type type{};
    std::future<Paths> fullPaths{};
    std::future<Distribution> distribution{};
    std::shared_ptr<Job::MetaData> metaData =
        std::make_shared<Job::MetaData>(
            0,
            std::numeric_limits<State>::max(),
            std::numeric_limits<State>::min(),
            std::numeric_limits<State>::max(),
            std::numeric_limits<State>::min()
        );
    std::stop_source stop{};
};
