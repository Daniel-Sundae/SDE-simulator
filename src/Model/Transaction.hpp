#pragma once
#include "PathQuery.hpp"

struct Transaction{
	const PathQuery pathQuery;
	const PathQuery deterministicQuery;
	const PDF pdf;
    Transaction(PathQuery _pathQuery, PathQuery _deterministicQuery, PDF _pdf)
        : pathQuery(std::move(_pathQuery))
		, deterministicQuery(std::move(_deterministicQuery))
		, pdf(std::move(_pdf))
	{}
};