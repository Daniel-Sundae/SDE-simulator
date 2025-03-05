#pragma once
#import <functional>
#import "Types.hpp"
struct PDFQuery {
    const std::function<Density(State)> pdf;
    const std::size_t points;
    const double increment;
    explicit PDFQuery(std::function<Density(State)> _pdf, std::size_t _points, double _increment)
        : pdf(_pdf)
        , points(_points)
        , increment(_increment)
    {
    }
};