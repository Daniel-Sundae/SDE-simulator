#pragma once

#include <future>
#include <atomic>
#include <stop_token>
#include <memory>
#include <limits>
#include <variant>
#include "Types.hpp"

struct Job {
    enum class Type {
        Deterministic,
        Stochastic,
        StochasticFullPath,
    };
    struct MetaData {
        std::atomic<size_t> pathsCompleted{0};
        std::atomic<State> minXT{std::numeric_limits<State>::max()};
        std::atomic<State> maxXT{std::numeric_limits<State>::lowest()};
        std::atomic<State> minXt{std::numeric_limits<State>::max()};
        std::atomic<State> maxXt{std::numeric_limits<State>::lowest()};
    };
protected:
    explicit Job(size_t _totalPaths, Type _type)
        : totalPaths(_totalPaths), type(_type)
    {}
    ~Job() = default;
public:
    const size_t totalPaths{};
    const Type type{};
    std::stop_source stop{};
    const std::shared_ptr<MetaData> metaData{ std::make_shared<MetaData>() };
};

struct DeterministicJob : public Job{
    explicit DeterministicJob(size_t _totalPaths)
        : Job(_totalPaths, Type::Deterministic)
    {}
    std::future<Path> drift{};
};

struct StochasticJob : public Job {
    explicit StochasticJob(size_t _totalPaths)
        : Job(_totalPaths, Type::Stochastic)
    {}
    std::future<Distribution> distribution{};
};

struct StochasticFullPathsJob : public Job {
    explicit StochasticFullPathsJob(size_t _totalPaths)
        : Job(_totalPaths, Type::StochasticFullPath)
    {}
    std::future<Paths> fullPaths{};
};

using AnyJob = std::variant<
    DeterministicJob,
    StochasticJob,
    StochasticFullPathsJob>;