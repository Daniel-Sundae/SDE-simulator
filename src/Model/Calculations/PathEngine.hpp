#pragma once
#include "EngineThreadPool.hpp"
#include "PathQuery.hpp"
#include "Job.hpp"
#include <memory>

class PathEngine{
public:
    explicit PathEngine() = default;
    [[nodiscard]] AnyJob processQuery(const AnyQuery &query);
private:
    std::unique_ptr<EngineThreadPool> m_tp =
        std::make_unique<EngineThreadPool>();
};
