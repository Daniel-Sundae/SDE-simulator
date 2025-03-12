#pragma once
#include <functional>
#include "Types.hpp"
struct PDFQuery {
    const PDF pdf;
    const std::size_t points;
    explicit PDFQuery(PDF _pdf, std::size_t _points)
        : pdf(_pdf)
        , points(_points)
    {}
};