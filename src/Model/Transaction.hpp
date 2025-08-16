#pragma once
#include "PathQuery.hpp"
struct Transaction{
    static inline constexpr size_t numQueries = 2;
    const PathQuery pathQuery;
    const PathQuery deterministicQuery;
    Transaction(PathQuery _pathQuery, PathQuery _deterministicQuery)
        : pathQuery(std::move(_pathQuery))
        , deterministicQuery(std::move(_deterministicQuery))
    {}
};