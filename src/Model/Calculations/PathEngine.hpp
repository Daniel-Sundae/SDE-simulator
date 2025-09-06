#pragma once
#include "Types.hpp"
#include "EngineThreadPool.hpp"
#include "Job.hpp"
#include <memory>
#include <functional>
#include <cmath>

struct PathQuery;
struct Transaction;

class PathEngine{
public:
    explicit PathEngine() = default;
    [[nodiscard]] AnyJob processQuery(const AnyQuery &query);
private:
    std::unique_ptr<EngineThreadPool> m_tp =
        std::make_unique<EngineThreadPool>();
};
