#pragma once
#include <functional>
#include "Types.hpp"
struct PDFQuery {
    const PDF pdf;
    const std::size_t points;
    const double increment;
    explicit PDFQuery(PDF _pdf, std::size_t _points, double _increment)
        : pdf(_pdf)
        , points(_points)
        , increment(_increment)
    {
    }
};