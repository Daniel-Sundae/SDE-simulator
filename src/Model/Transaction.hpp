#pragma once
#include "PathQuery.hpp"
struct Transaction{
    const PathQuery pathQuery;
    const PathQuery deterministicQuery;
    Transaction(PathQuery _pathQuery, PathQuery _deterministicQuery)
        : pathQuery(std::move(_pathQuery))
        , deterministicQuery(std::move(_deterministicQuery))
    {}
};