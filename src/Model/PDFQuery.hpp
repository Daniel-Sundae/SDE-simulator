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
        pdfWithNoData.GeneratePDFData(points);
        return pdfWithNoData;
    }
};