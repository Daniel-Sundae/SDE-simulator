#pragma once
#include "Types.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include <vector>
#include <functional>
#include <optional>
struct PDF {
    State EV = 0;
    double stddev = 0;
    std::function<Density(State)> pdf = nullptr;
    std::optional<PDFData> data = std::nullopt;
    static constexpr size_t pdfPoints = DefaultConstants::pdfPoints;
    PDF() = default;
    PDF(double _EV, double _stddev, std::function<Density(State)> _pdf)
        : EV(_EV)
        , stddev(_stddev)
        , pdf(_pdf)
    {
    }
    Density operator()(State s) const{ return pdf(s); }
    explicit operator bool() const{ return pdf ? true : false; }
    void generatePDFData(const Range support){
        Utils::assertTrue(support.first < support.second, "Invalid support range");
        data.emplace();
        data->reserve(pdfPoints);
        const State start = support.first;
        const State end = support.second;
        const double increment = (end - start) / static_cast<double>(pdfPoints);
        for (State state = start; state < end; state += increment) {
            data->push_back(pdf(state));
        }
    }
};