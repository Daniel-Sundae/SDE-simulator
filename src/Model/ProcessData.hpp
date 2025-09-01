#pragma once
#include "Types.hpp"
#include "PDF.hpp"
#include "DriftDiffusion.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <concepts>
#include <string_view>
#include <cmath>

struct Constants {
    const Range allowedValues;
    const double defaultValue;
    const double incrementSize;
};

template <ProcessType Type>
struct Fields;

struct FieldTags {
    struct name {};
    struct acronym {};
    struct description {};
    struct definition {};
    struct muData {};
    struct sigmaData {};
    struct startValue {};
    struct drift {};
    struct diffusion {};
    struct pdf {};
};

template<ProcessType P>
decltype(auto) pick(FieldTags::name) { return Fields<P>::name; }

template<ProcessType P>
decltype(auto) pick(FieldTags::acronym) { return Fields<P>::acronym; }

template<ProcessType P>
decltype(auto) pick(FieldTags::description) { return Fields<P>::description; }

template<ProcessType P>
decltype(auto) pick(FieldTags::definition) { return Fields<P>::definition; }

template<ProcessType P>
decltype(auto) pick(FieldTags::muData) { return Fields<P>::muData; }

template<ProcessType P>
decltype(auto) pick(FieldTags::sigmaData) { return Fields<P>::sigmaData; }

template<ProcessType P>
decltype(auto) pick(FieldTags::startValue) { return Fields<P>::startValue; }

template<ProcessType P, class... Args>
decltype(auto) pick(FieldTags::drift, Args&&... args) { return Fields<P>::drift(std::forward<Args>(args)...); }

template<ProcessType P, class... Args>
decltype(auto) pick(FieldTags::diffusion, Args&&... args) { return Fields<P>::diffusion(std::forward<Args>(args)...); }

template<ProcessType P, class... Args>
decltype(auto) pick(FieldTags::pdf, Args&&... args) { return Fields<P>::pdf(std::forward<Args>(args)...); }

template <typename FieldTag, typename... Args>
decltype(auto) getField(FieldTag, ProcessType p, Args&&... args) {
    switch (p) {
    case ProcessType::BM:
        return pick<ProcessType::BM>(FieldTag{}, std::forward<Args>(args)...);
    case ProcessType::GBM:
        return pick<ProcessType::GBM>(FieldTag{}, std::forward<Args>(args)...);
    case ProcessType::OU:
        return pick<ProcessType::OU>(FieldTag{}, std::forward<Args>(args)...);
    default:
        Utils::fatalError("Unknown process type: {}", static_cast<int>(p));
    }
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
        static_assert(std::is_same_v <decltype(ProcessT::startValue), const Constants>);
    }
};

template <>
struct Fields<ProcessType::BM> : RequiredFields<Fields<ProcessType::BM>> {
    static constexpr std::string_view name = "Brownian Motion";
    static constexpr std::string_view acronym = "BM";
    static constexpr std::string_view description = "Standard brownian motion.";
    static constexpr std::string_view definition = "dX = μdt + σdB";
    static constexpr Constants muData{ {-0.5, 0.5}, 0, 0.1 };
    static constexpr Constants sigmaData{ {0, 3}, 0.2, 0.1 };
    static constexpr Constants startValue{ {-100, 100}, 0, 1 };
    static auto drift(const double _mu) -> Drift {
        return Drift(_mu, [_mu](Time, State) { return _mu; });
    };
    static auto diffusion(const double _sigma) -> Diffusion {
        return Diffusion(_sigma, [_sigma](Time, State) { return _sigma; });
    };
    static auto pdf(const State _startValue, const Time time, const double _mu, const double _sigma) -> PDF {
        const double EV = _startValue + _mu * time;
        const double stddev = _sigma * std::sqrt(time);
        const double denominator = std::sqrt(2.0 * DefaultConstants::PI * time) * _sigma;
        const double expDenominator = 2.0 * _sigma * _sigma * time;
        const auto _pdf = [=](const State XT) -> Density {
            const double expNumerator = - std::pow(XT - (_startValue + _mu * time), 2);
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
    static constexpr Constants muData{ {-0.5, 0.5}, 0.2, 0.1 };
    static constexpr Constants sigmaData{ {0.01, 1.2}, 0.2, 0.05 };
    static constexpr Constants startValue{ {0.1, 2}, 1, 0.1 };
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
    static auto pdf(const State _startValue, const Time time, const double _mu, const double _sigma) -> PDF {
        const double EV = _startValue * std::exp(_mu * time);
        const double stddev = EV * sqrt(exp(_sigma * _sigma * time) - 1);
        const double expDenominator = 2.0 * _sigma * _sigma * time;      
        const auto _pdf = [=](const State XT) -> Density {
            if (XT <= 0.0) {
                return 0;
            }
            const double expNumerator = - std::pow(std::log(XT / _startValue) - (_mu - 0.5 * _sigma * _sigma) * time, 2);
            const double denominator = std::sqrt(2 * DefaultConstants::PI * time) * XT * _sigma;
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
    static constexpr Constants startValue{ {-50, 50}, 1, 1 };

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

    static auto pdf(const State _startValue, const Time time, const double _mu, const double _sigma) -> PDF {
        const double EV = _startValue * std::exp(-DefaultConstants::OUthetaData * time) + _mu * (1.0 - std::exp(-DefaultConstants::OUthetaData * time));
        const double _privateRepeatedVal = (_sigma * _sigma) / (2.0 * DefaultConstants::OUthetaData) * (1.0 - std::exp(-2.0 * DefaultConstants::OUthetaData * time));
        const double stddev = std::sqrt(_privateRepeatedVal);
        const double expDenominator = 2.0 * _privateRepeatedVal;
        const auto _pdf = [=](const State XT) -> Density {
            const double expNumerator = -std::pow(XT - EV, 2);
            const double denominator = std::sqrt(2.0 * DefaultConstants::PI * _privateRepeatedVal);
            return (1.0 / denominator) * std::exp(expNumerator / expDenominator);
        };
        return PDF(EV, stddev, _pdf);
    };
};
