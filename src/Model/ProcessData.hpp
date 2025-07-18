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

    template <ProcessType Type>
    struct Fields;
public:
    template <ProcessType Type>
    static std::string_view getName(){
        return Fields<Type>::name;
    }

    template <ProcessType Type>
    static std::string_view getAcronym() {
        return Fields<Type>::acronym;
    }

    template <ProcessType Type>
    static std::string_view getDescription() {
        return Fields<Type>::description;
    }

    template <ProcessType Type>
    static std::string_view getDefinition() {
        return Fields<Type>::definition;
    }

    template <ProcessType Type>
    static Constants getMuData() {
        return Fields<Type>::muData;
    }

    template <ProcessType Type>
    static Constants getSigmaData() {
        return Fields<Type>::sigmaData;
    }

    template <ProcessType Type>
    static Constants getStartValueData() {
        return Fields<Type>::startValueData;
    }

    template <ProcessType Type>
    static Drift getDrift(const double _mu) {
        return Fields<Type>::drift(_mu);
    }

    template <ProcessType Type>
    static Diffusion getDiffusion(const double _sigma) {
        return Fields<Type>::diffusion(_sigma);
    }

    template <ProcessType Type>
    static PDF getPDF(const State startValue, const Time time,
                      const double _mu, const double _sigma) {
        return Fields<Type>::pdf(startValue, time, _mu, _sigma);
    }

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

    template <>
    struct Fields<ProcessType::BM> : RequiredFields<Fields<ProcessType::BM>> {
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

    template <>
    struct Fields<ProcessType::GBM> : RequiredFields<Fields<ProcessType::GBM>> {
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

    template <>
    struct Fields<ProcessType::OU> : RequiredFields<Fields<ProcessType::OU>> {
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
};