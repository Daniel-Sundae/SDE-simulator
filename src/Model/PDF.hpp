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
    std::optional<std::function<Density(State)>> pdf = std::nullopt;
    std::optional<PDFData> data = std::nullopt;
    static constexpr size_t pdfPoints = DefaultConstants::pdfPoints;
    PDF() = default;
    PDF(double _EV, double _stddev, std::optional<std::function<Density(State)>> _pdf)
        : EV(_EV)
        , stddev(_stddev)
        , pdf(_pdf)
    {
    }

    Density operator()(State s) const{ return pdf ? (*pdf)(s) : 0.0; }
    void generatePDFData(const Range support){
        Utils::assertTrue(support.first < support.second, "Invalid support range");
        data.emplace();
        data->reserve(pdfPoints);
        const State start = support.first;
        const State end = support.second;
        const double increment = (end - start) / static_cast<double>(pdfPoints);
        for (State state = start; state < end; state += increment) {
            data->push_back(pdf ? (*pdf)(state) : 0.0);
        }
    }
    Range support(
            const std::optional<State> minPointToInclude,
            const std::optional<State> maxPointToInclude) const {
        // Denote Riemann rectangle at Xt is pdf(Xt) * width.
        // Assuming every rectangle is area of this rectangle, threshold states how large fraction
        // of the whole area, 1, that is needed for Xt to be considered a point with support.
        State threshold = 0.001;
        State padding = 0.001; // In case stddev = 0
        State start = EV - 3 * stddev - padding;
        if (minPointToInclude) start = std::min(start, *minPointToInclude);
        State end = EV + 3 * stddev + padding;
        if (maxPointToInclude) end = std::max(end, *maxPointToInclude);
        if (pdf == std::nullopt) {
            return {start, end};
        }
        State lower = start;
        State upper = end;
        const double increment = (end - start) / static_cast<double>(pdfPoints);
        // Find lower and upper bound if pdf is available
        for (State state = start; state < end; state += increment) {
            if ((*pdf)(state) * increment * pdfPoints > threshold) {
                lower = state;
                if (minPointToInclude.has_value())
                    lower = std::min(lower, minPointToInclude.value());
                break;
            }
        }
        for (State state = end; lower < state; state -= increment) {
            if ((*pdf)(state) * increment * pdfPoints > threshold) {
                upper = state;
                if (maxPointToInclude.has_value())
                    upper = std::max(upper, maxPointToInclude.value());
                break;
            }
        }
        if (lower >= upper){
            Utils::fatalError("Failed to generate PDF support: (lower, upper) is ({}, {})",
                lower, upper);
        }
        return {lower, upper};
    }
};