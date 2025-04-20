#pragma once
#include "PathQuery.hpp"
#include "PDFQuery.hpp"

struct Transaction{
	const PathQuery pathQuery;
	const PathQuery deterministicQuery;
	const PDFQuery pdfQuery;
    Transaction(PathQuery _pathQuery, PathQuery _deterministicQuery, PDFQuery _pdfQuery)
        : pathQuery(std::move(_pathQuery))
		, deterministicQuery(std::move(_deterministicQuery))
		, pdfQuery(std::move(_pdfQuery))
	{}
};