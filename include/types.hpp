#pragma once
#include <vector>
#include <optional>
#include <stdexcept>
#include <functional>
#include <cstddef>
#include <cassert>
#include <cstdint>

using Time = double;
using State = double;
using Density = double;
using Path = std::vector<State>;
using Paths = std::vector<Path>;
using Distribution = std::vector<State>;
using PDFData = std::vector<Density>;
using StateDot = double; // dX/dt
using Range = std::pair<double, double>;
template <typename T>
concept IntOrDouble = std::same_as<T, int> || std::same_as<T, double>;

class FunctionWrapper {
protected:
    const double parameter;
    const std::function<StateDot(Time, State)> f;
public:
    FunctionWrapper()
        : parameter(0)
        , f([](Time, State) -> StateDot { return 0; })
    {}
    FunctionWrapper(double _parameter, std::function<StateDot(Time, State)> _f)
        : parameter(_parameter)
        , f(_f)
    {}
    auto operator()(Time t, State s) const -> StateDot { return f(t, s); }
};

class Drift : public FunctionWrapper {
    using FunctionWrapper::FunctionWrapper;
public:
    auto Mu() const -> double { return parameter; }
};

class Diffusion : public FunctionWrapper {
    using FunctionWrapper::FunctionWrapper;
public:
    auto Sigma() const -> double { return parameter; }
};

class PDF {
    const State m_EV;
    const double m_stddev;
    const std::function<Density(State)> m_pdf;
    mutable PDFData m_data;
    mutable Range m_support;
    static constexpr double threshold = 1e-6;

public:
    PDF(double _EV, double _stddev, std::function<Density(State)> _pdf)
        : m_EV(_EV)
        , m_stddev(_stddev)
        , m_pdf(_pdf)
        , m_data({})
        , m_support(std::make_pair(0,0))
    {}
    auto operator()(State s) const -> Density { return m_pdf(s); }
    explicit operator bool() const {
        return m_pdf ? true : false;
    }
    auto EV() const -> double{
        return m_EV;
    }
    auto StdDev() const -> double{
        return m_stddev;
    }

    auto GetPDFData() const -> std::optional<const PDFData>{
        if (m_data.empty()) {
            return std::nullopt;
        };
        return m_data;
    }

    auto GeneratePDFData(const std::size_t points) const -> PDFData{
        PDFData newData;
        newData.reserve(points);
        const auto [start, end] = GetSupport();
        const double increment = (end - start)/static_cast<double>(points);
        for (State state = start; state < end; state += increment){
            newData.push_back(m_pdf(state));
        }
        m_data = newData;
        return newData;
    }

    auto GetSupport() const -> Range{
        if(!(m_support.first == 0 && m_support.second == 0))
            return m_support;

        std::size_t points = 1000;
        const State start = EV() - 5*StdDev();
        const State end = EV() + 5*StdDev();
        m_support.first = start;
        m_support.second = end;
        const double increment = (end - start) / static_cast<double>(points);
        // Find lower bound
        for (State state = start; state < end; state += increment){
            if(m_pdf(state) > threshold){
                m_support.first = state;
                break;
            }
        }
        // Find upper bound
        for (State state = end; start < state; state -= increment){
            if(m_pdf(state) > threshold){
                m_support.second = state;
                break;
            }
        }
        assert(m_support.first <= m_support.second);
        return std::make_pair(m_support.first, m_support.second);
    }
};

enum class ProcessType{
    NONE = 0,
    CUSTOM,
    BM,
    GBM,
    Levy,
    OU,
    BB,
};

enum class SolverType {
    EULER_MARUYAMA = 0,
    RUNGE_KUTTA,
};

enum class ActionWidget {
    GO = 0,
    CLEAR,
};

enum class DefinitionWidget {
    PROCESS = 0,
    MU,
    SIGMA,
    STARTVALUE
};

enum class SimulationWidget {
    SOLVER = 0,
    TIME,
    dt,
    SAMPLES,
};

enum class SettingsWidget {
    THREADS = 0,
    AUTOUPDATE,
    FIXSEED,
};

