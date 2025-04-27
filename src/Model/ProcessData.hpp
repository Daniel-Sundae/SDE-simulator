#pragma once
#include "Types.hpp"
#include "PDF.hpp"
#include "DriftDiffusion.hpp"
#include "Constants.hpp"
#include <stdexcept>
#include <concepts>
#include <string_view>
#include <cmath>

class ProcessData {
public:

    struct Constants {
        const Range allowedValues;
        const double defaultValue;
        const double incrementSize;
    };

    using DiffusionFunction = std::function<Diffusion(const double)>;

    template <typename ProcessT>
    struct RequiredFields {
        constexpr RequiredFields(){
            static_assert(std::is_same_v <decltype(ProcessT::name), const std::string_view>);
            static_assert(std::is_same_v <decltype(ProcessT::acronym), const std::string_view>);
            static_assert(std::is_same_v <decltype(ProcessT::description), const std::string_view>);
            static_assert(std::is_same_v <decltype(ProcessT::definition), const std::string_view>);
            static_assert(std::is_same_v <decltype(ProcessT::muData), const Constants>);
            static_assert(std::is_same_v <decltype(ProcessT::sigmaData), const Constants>);
            static_assert(std::is_same_v <decltype(ProcessT::startValueData), const Constants>);
        }
    };

    struct BM : RequiredFields<BM> {
        static constexpr std::string_view name = "Brownian Motion";
        static constexpr std::string_view acronym = "BM";
        static constexpr std::string_view description = "Standard brownian motion.";
        static constexpr std::string_view definition = "dX = μdt + σdB";
        static constexpr Constants muData{ {-0.5,0.5},0,0.1 };
        static constexpr Constants sigmaData{ {0,1.2},0.2,0.1 };
        static constexpr Constants startValueData{ {-100, 100}, 0,1 };
        static auto drift(const double _mu) -> Drift {
            return Drift(_mu, [_mu](Time, State) { return _mu; });
        };
        static auto diffusion(const double _sigma) -> Diffusion {
            return Diffusion(_sigma, [_sigma](Time, State) { return _sigma; });
        };
        static auto pdf(const State startValue, const Time time, const double _mu, const double _sigma) -> PDF {
            const double EV = startValue + _mu * time;
            const double stddev = _sigma * std::sqrt(time);
            const double denominator = std::sqrt(2.0 * DefaultConstants::PI * time) * _sigma;
            const double expDenominator = 2.0 * _sigma * _sigma * time;
            const auto _pdf = [=](const State endValue) -> Density {
                const double expNumerator = - std::pow(endValue - (startValue + _mu * time), 2);
                const double exponent = expNumerator / expDenominator;
                return (1 / denominator) * std::exp(exponent);
                };
            return PDF(EV, stddev, _pdf);
        };
    };

    struct GBM : RequiredFields<GBM> {
        static constexpr std::string_view name = "Geometric Brownian Motion";
        static constexpr std::string_view acronym = "GBM";
        static constexpr std::string_view description = "Geometric brownian motion.";
        static constexpr std::string_view definition = "dX = μXdt + σXdB";
        static constexpr Constants muData{ {-0.5,0.5},0,0.1 };
        static constexpr Constants sigmaData{ {0,1},0.2,0.1 };
        static constexpr Constants startValueData{ {0.1, 100},1,1 };
        static auto drift(const double _mu) -> Drift {
            return Drift(
                _mu,
                [_mu](Time, State Xt) { return _mu * Xt; },
                [_mu](Time, State) { return _mu; }
            );
        };
        static auto diffusion(const double _sigma) -> Diffusion {
            return Diffusion(
                _sigma,
                [_sigma](Time, State Xt) { return _sigma * Xt; },
                [_sigma](Time, State) { return _sigma; }
            );
        };
        static auto pdf(const State startValue, const Time time, const double _mu, const double _sigma) -> PDF {
            const double EV = startValue * std::exp(_mu * time);
            const double stddev = EV * sqrt(exp(_sigma * _sigma * time) - 1);
            const double expDenominator = 2.0 * _sigma * _sigma * time;      
            const auto _pdf = [=](const State endValue) -> Density {
                if (endValue <= 0.0) {
                    return 0;
                }
                const double expNumerator = - std::pow(std::log(endValue / startValue) - (_mu - 0.5 * _sigma * _sigma) * time, 2);
                const double denominator = std::sqrt(2 * DefaultConstants::PI * time) * endValue * _sigma;
                return (1 / denominator) * std::exp(expNumerator / expDenominator);
            };
            return PDF(EV, stddev, _pdf);
        };
    };

    struct OU : RequiredFields<OU> {
        static constexpr std::string_view name = "Ornstein-Uhlenbeck Process";
        static constexpr std::string_view acronym = "OU";
        static constexpr std::string_view description = "Ornstein-Uhlenbeck process (mean-reverting with θ = 1).";
        static constexpr std::string_view definition = "dX = θ(μ - X)dt + σdB";
        static constexpr Constants muData{ {-10, 10}, 0, 0.1 };
        static constexpr Constants sigmaData{ {0.01, 2}, 0.2, 0.1 };
        static constexpr Constants startValueData{ {-50, 50}, 1, 1 };
    
        static auto drift(const double _mu) -> Drift {
            return Drift(
                _mu,
                [_mu](Time, State Xt) { return DefaultConstants::OUthetaData * (_mu - Xt); },
                [_mu](Time, State) { return DefaultConstants::OUthetaData * -1; }
            );
        };
    
        static auto diffusion(const double _sigma) -> Diffusion {
            return Diffusion(_sigma, [_sigma](Time, State) { return _sigma; });
        };

        static auto pdf(const State startValue, const Time time, const double _mu, const double _sigma) -> PDF {
            const double EV = startValue * std::exp(-DefaultConstants::OUthetaData * time) + _mu * (1.0 - std::exp(-DefaultConstants::OUthetaData * time));
            const double _privateRepeatedVal = (_sigma * _sigma) / (2.0 * DefaultConstants::OUthetaData) * (1.0 - std::exp(-2.0 * DefaultConstants::OUthetaData * time));
            const double stddev = std::sqrt(_privateRepeatedVal);
            const double expDenominator = 2.0 * _privateRepeatedVal;
            const auto _pdf = [=](const State endValue) -> Density {
                const double expNumerator = -std::pow(endValue - EV, 2);
                const double denominator = std::sqrt(2.0 * DefaultConstants::PI * _privateRepeatedVal);
                return (1.0 / denominator) * std::exp(expNumerator / expDenominator);
            };
            return PDF(EV, stddev, _pdf);
        };
    };

private:
    template <auto Field>
    static auto GetField(ProcessType type) {
        switch (type) {
        case ProcessType::BM:
            return Field(ProcessData::BM{});
        case ProcessType::GBM:
            return Field(ProcessData::GBM{});
        case ProcessType::OU:
           return Field(ProcessData::OU{});
        default:
            throw std::runtime_error("Unknown process type");
        }
    }

#define GET_PROCESS_FIELD(field) ProcessData::GetField<[](auto t) { return decltype(t)::field; }>(type)
public:
    static auto GetName(ProcessType type) -> std::string_view
    {
        return GET_PROCESS_FIELD(name);
    }
    static auto GetAcronym(ProcessType type) -> std::string_view
    {
        return GET_PROCESS_FIELD(acronym);
    }
    static auto GetDescription(ProcessType type) -> std::string_view
    {
        return GET_PROCESS_FIELD(description);
    }
    static auto GetDefinition(ProcessType type) -> std::string_view
    {
        return GET_PROCESS_FIELD(definition);
    }
    static auto GetMuData(ProcessType type) -> Constants
    {
        return GET_PROCESS_FIELD(muData);
    }
    static auto GetSigmaData(ProcessType type) -> Constants
    {
        return GET_PROCESS_FIELD(sigmaData);
    }
    static auto GetStartValueData(ProcessType type) -> Constants
    {
        return GET_PROCESS_FIELD(startValueData);
    }
    static auto GetDrift(ProcessType type, const double _mu) -> Drift
    {
        return GET_PROCESS_FIELD(drift)(_mu);
    }
    static auto GetDiffusion(ProcessType type, const double _sigma) -> Diffusion
    {
        return GET_PROCESS_FIELD(diffusion)(_sigma);
    }
    static auto GetPDF(ProcessType type, const State startValue, const Time time, const double _mu, const double _sigma) -> PDF
    {
        return GET_PROCESS_FIELD(pdf)(startValue, time, _mu, _sigma);
    }
#undef GET_PROCESS_FIELD
};