#pragma once
#include "Types.hpp"
#include "Constants.hpp"
#include <functional>
struct PDF {
    const State m_EV;
    const double m_stddev;
    const std::function<Density(State)> m_pdf;
    PDFData m_data;
    Range m_support;
    static constexpr double threshold = DefaultConstants::pdfThreshold;

public:
    PDF(double _EV, double _stddev, std::function<Density(State)> _pdf)
        : m_EV(_EV)
        , m_stddev(_stddev)
        , m_pdf(_pdf)
        , m_data({})
        , m_support(std::make_pair(0, 0))
    {
        m_data = GeneratePDFData(1000); // Do this after initializing since affects m_support
    }
    auto operator()(State s) const -> Density { return m_pdf(s); }
    explicit operator bool() const
    {
        return m_pdf ? true : false;
    }

    auto EV() const -> double
    {
        return m_EV;
    }

    auto StdDev() const -> double
    {
        return m_stddev;
    }

    auto GetPDFData() const -> PDFData
    {
        return m_data;
    }

    auto GetSupport() const -> Range{
        return m_support;
    }

private:
    auto GeneratePDFData(const std::size_t points) -> PDFData
    {
        PDFData result = {};
        result.reserve(points);
        const auto [start, end] = GenerateSupport(points);
        const double increment = (end - start) / static_cast<double>(points);
        for (State state = start; state < end; state += increment) {
            result.push_back(m_pdf(state));
        }
        return result;
    }

    auto GenerateSupport(const std::size_t points) -> Range
    {
        State start = EV() - 5 * StdDev();
        State end = EV() + 5 * StdDev();
        // In case user set stddev = 0
        if (abs(end-start) < threshold ){
            start -= 1;
            end += 1;
        }
        m_support.first = start;
        m_support.second = end;
        const double increment = (end - start) / static_cast<double>(points);
        // Find lower bound
        for (State state = start; state < end; state += increment) {
            if (m_pdf(state) > threshold) {
                m_support.first = state;
                break;
            }
        }
        // Find upper bound
        for (State state = end; start < state; state -= increment) {
            if (m_pdf(state) > threshold) {
                m_support.second = state;
                break;
            }
        }
        assert(m_support.first <= m_support.second);
        return std::make_pair(m_support.first, m_support.second);
    }
};