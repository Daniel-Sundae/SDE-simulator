#pragma once
#include <functional>
#include "Types.hpp"
struct PDFQuery {
    const PDF pdf;
    const std::size_t points;
    explicit PDFQuery(PDF _pdf, std::size_t _points)
        : pdf(PDFWithData(std::move(_pdf), _points))
        , points(_points)
    {}
private:
    static auto PDFWithData(PDF pdfWithNoData, std::size_t points) -> PDF
    {
        pdfWithNoData.GeneratePDFData(std::max(points, static_cast<std::size_t>(1000)));
        return pdfWithNoData;
    }
};