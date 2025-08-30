#pragma once
#include "PathQuery.hpp"
struct Transaction{
    const DeterministicQuery deterministicQuery;
    const StochasticQuery stochasticQuery;
    const StochasticFullPathsQuery stochasticFullPathQuery;
    Transaction(DeterministicQuery _deterministicQuery,
            StochasticQuery _stochasticQuery,
            StochasticFullPathsQuery _stochasticFullPathQuery)
        : deterministicQuery(std::move(_deterministicQuery))
        , stochasticQuery(std::move(_stochasticQuery))
        , stochasticFullPathQuery(std::move(_stochasticFullPathQuery))
    {}
};