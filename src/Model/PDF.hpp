#pragma once
#include "Types.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include <vector>
#include <functional>
struct PDF {
    const State m_EV;
    const double m_stddev;
    const std::function<Density(State)> m_pdf;
    PDFData m_data;
    Range m_support;
    static constexpr double threshold = DefaultConstants::pdfThreshold;
    static constexpr size_t pdfPoints = DefaultConstants::pdfPoints;

private:
    PDF(double _EV, double _stddev, std::function<Density(State)> _pdf)
        : m_EV(_EV)
        , m_stddev(_stddev)
        , m_pdf(_pdf)
    {
    }
public:
    static PDF create(double _EV, double _stddev, std::function<Density(State)> _pdf) {
        PDF pdf(_EV, _stddev, _pdf);
        pdf.generateSupport();
        pdf.generatePDFData();
        return pdf;
    }
    Density operator()(State s) const{ return m_pdf(s); }
    explicit operator bool() const{ return m_pdf ? true : false; }
    double EV() const{ return m_EV; }
    double stdDev() const{ return m_stddev; }
    PDFData getPDFData() const{ return m_data; }
    Range getSupport() const{ return m_support; }

private:
    void generatePDFData(){
        m_data.reserve(pdfPoints);
        const State start = m_support.first;
        const State end = m_support.second;
        const double increment = (end - start) / static_cast<double>(pdfPoints);
        for (State state = start; state < end; state += increment) {
            m_data.push_back(m_pdf(state));
        }
    }

    void generateSupport(){
        State start = EV() - 5 * stdDev();
        State end = EV() + 5 * stdDev();
        // In case user set stddev = 0
        if (abs(end-start) < threshold ){
            start -= 1;
            end += 1;
        }
        m_support.first = start;
        m_support.second = end;
        const double increment = (end - start) / static_cast<double>(pdfPoints);
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
        if(m_support.first > m_support.second){
            Utils::fatalError("Failed to generate PDF support: (start, end) is ({}, {})",
                m_support.first, m_support.second);
        }
    }
};