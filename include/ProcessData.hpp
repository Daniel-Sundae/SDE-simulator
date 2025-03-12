#pragma once
#include "Types.hpp"
#include "DefaultConstants.hpp"
#include <stdexcept>
#include <concepts>
#include <string_view>

// TODO: Make all these namespaces
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
        static constexpr std::string_view definition = "dX = σdB";
        static constexpr Constants muData{ {0,0},0,0.1 };
        static constexpr Constants sigmaData{ {0,1.2},0.2,0.1 };
        static constexpr Constants startValueData{ {-100,100},0,1 };
        static auto drift(const double _mu) -> Drift {
            // _mu is unused but necessary for uniform lambda signature.
            // Supress compiler warning.
            (void)_mu;
            return Drift(0, [](Time, State) -> StateDot { return 0.0; });
        };
        static auto diffusion(const double _sigma) -> Diffusion {
            return Diffusion(_sigma, [_sigma](Time, State) { return _sigma; });
        };
        static auto pdf(const State startValue, const Time time, const double _mu, const double _sigma) -> PDF {
            const double EV = startValue + _mu * time;
            //const double EV = startValue; // This is incorrect EV
            const double stddev = _sigma * std::sqrt(time);
            // Pre computed constants
            const double sqrt_2pi = std::sqrt(2.0 * PI);
            const double sigma_t = _sigma * std::sqrt(time);
            const double variance_t = _sigma * _sigma * time;
            const double two_variance_t = 2.0 * variance_t;
            const double drift_term = _mu * time;
            const auto _pdf = [=](const State endValue) -> Density {
                const double diff = endValue - startValue - drift_term;
                const double diff_squared = diff * diff;
                const double denominator = sigma_t * sqrt_2pi;
                const double exponent = -diff_squared / two_variance_t;
                return std::exp(exponent) / denominator;
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
        static constexpr Constants startValueData{ {0.1,100},1,1 };
        static auto drift(const double _mu) -> Drift {
            return Drift(_mu, [_mu](Time, State s) { return _mu * s; });
            };
        static auto diffusion(const double _sigma) -> Diffusion {
            return Diffusion(_sigma, [_sigma](Time, State s) { return _sigma * s; });
            };
        // Theoretical solution for GBM:
        // f(s)=\frac{1}{s \sqrt{2 \pi \sigma^2 T}} \exp \left[-\frac{\left(\ln \left(\frac{s}{S_0}\right)-\left(\mu-\frac{\sigma^2}{2}\right) T\right)^2}{2 \sigma^2 T}\right]
        static auto pdf(const State startValue, const Time time, const double _mu, const double _sigma) -> PDF {
            const double EV = startValue * std::exp(_mu * time);
            const double stddev = EV * sqrt(exp(_sigma * _sigma * time) - 1);
            // Pre computed constants
            const double sigma_squared = _sigma * _sigma;
            const double adjusted_drift = _mu - 0.5 * sigma_squared;
            const double sqrt_2pi = std::sqrt(2.0 * PI);
            const double sigma_t = _sigma * std::sqrt(time);
            const double variance_t = sigma_squared * time;
            const double two_variance_t = 2.0 * variance_t;
            const auto _pdf = [=](const State endValue) -> Density {
                if (endValue <= 0.0) {
                    return 0;
                }
                const double log_ratio = std::log(endValue / startValue);
                const double drift_term = adjusted_drift * time;
                const double diff = log_ratio - drift_term;
                const double diff_squared = diff * diff;
                const double denominator = endValue * sigma_t * sqrt_2pi;
                const double exponent = -diff_squared / two_variance_t;
                return std::exp(exponent) / denominator;
                };
            return PDF(EV, stddev, _pdf);
            };
    };

    /*static auto GetDrift(ProcessType processType, const double _mu) -> Drift
    {
        switch (processType)
        {
            case ProcessType::NONE:
                throw std::invalid_argument("Not implemented yet");
                break;
            case ProcessType::CUSTOM:
                throw std::invalid_argument("Not implemented yet");
            case ProcessType::BM:
                return BM::GetDrift();
                break;
            case ProcessType::GBM:
                return GBM::GetDrift(_mu);
                break;
            default:
                throw std::invalid_argument("Not implemented yet");
            }
    }

    static auto GetDiffusion(ProcessType processType, const double _sigma) -> Diffusion
    {
        switch (processType)
        {
        case ProcessType::NONE:
            throw std::invalid_argument("Not implemented yet");
            break;
        case ProcessType::CUSTOM:
            throw std::invalid_argument("Not implemented yet");
        case ProcessType::BM:
            return BM::Diffusion(_sigma);
            break;
        case ProcessType::GBM:
            return GBM::GetDiffusion(_sigma);
            break;
        default:
            throw std::invalid_argument("Not implemented yet");
        }
    }*/

private:
    template <auto Field>
    static auto GetField(ProcessType type) {
        switch (type) {
        case ProcessType::BM:
            return Field(ProcessData::BM{});
        case ProcessType::GBM:
            return Field(ProcessData::GBM{});
        case ProcessType::BB:
        case ProcessType::NONE:
        case ProcessType::CUSTOM:
        case ProcessType::Levy:
        case ProcessType::OU:
            throw std::runtime_error("Process type not implemented");
        }
        throw std::runtime_error("Unknown process type");
    }

#define GET_STATIC_FIELD(field) ProcessData::GetField<[](auto t) { return decltype(t)::field; }>(type)
public:
    static auto GetName(ProcessType type) -> std::string_view
    {
        return GET_STATIC_FIELD(name);
    }
    static auto GetAcronym(ProcessType type) -> std::string_view
    {
        return GET_STATIC_FIELD(acronym);
    }
    static auto GetDescription(ProcessType type) -> std::string_view
    {
        return GET_STATIC_FIELD(description);
    }
    static auto GetDefinition(ProcessType type) -> std::string_view
    {
        return GET_STATIC_FIELD(definition);
    }
    static auto GetMuData(ProcessType type) -> Constants
    {
        return GET_STATIC_FIELD(muData);
    }
    static auto GetSigmaData(ProcessType type) -> Constants
    {
        return GET_STATIC_FIELD(sigmaData);
    }
    static auto GetStartValueData(ProcessType type) -> Constants
    {
        return GET_STATIC_FIELD(startValueData);
    }
    static auto GetDrift(ProcessType type, const double _mu) -> Drift
    {
        return GET_STATIC_FIELD(drift)(_mu);
    }
    static auto GetDiffusion(ProcessType type, const double _sigma) -> Diffusion
    {
        return GET_STATIC_FIELD(diffusion)(_sigma);
    }
    static auto GetPDF(ProcessType type, const State startValue, const Time time, const double _mu, const double _sigma) -> PDF
    {
        return GET_STATIC_FIELD(pdf)(startValue, time, _mu, _sigma);
    }
#undef GET_STATIC_FIELD

};



//static inline auto GetDrift() -> Drift {
        //    return Drift(0, [](Time, State) -> StateDot { return 0.0; });
        //}
        //static inline auto GetDiffusion(const double _sigma) -> Diffusion {
        //    return Diffusion(_sigma, [_sigma](Time, State) { return _sigma; });
        //}
        //// Theoretical solution for BM:
        //// f(s)=\frac{1}{\sqrt{2 \pi \sigma^2 T}} \exp \left[-\frac{(s - S_0 - \mu T)^2}{2 \sigma^2 T}\right]
        //static inline auto GetPDF(const State startValue, const Time time, const double _mu, const double _sigma) -> PDF {

        //    //const double EV = startValue + _mu * time;
        //    const double EV = startValue; // This error should be visible!
        //    const double stddev = _sigma * std::sqrt(time);

        //    // Pre computed constants
        //    const double sqrt_2pi = std::sqrt(2.0 * PI);
        //    const double sigma_t = _sigma * std::sqrt(time);
        //    const double variance_t = _sigma * _sigma * time;
        //    const double two_variance_t = 2.0 * variance_t;
        //    const double drift_term = _mu * time;
        //    const auto _pdf = [=](const State endValue) -> Density {
        //        const double diff = endValue - startValue - drift_term;
        //        const double diff_squared = diff * diff;
        //        const double denominator = sigma_t * sqrt_2pi;
        //        const double exponent = -diff_squared / two_variance_t;
        //        return std::exp(exponent) / denominator;
        //        };
        //    return PDF(EV, stddev, _pdf);
        //}


//static inline auto GetDrift(const double _mu) -> Drift {
//    return Drift(_mu, [_mu](Time, State s) { return _mu * s; });
//}
//static inline auto GetDiffusion(const double _sigma) -> Diffusion {
//    return Diffusion(_sigma, [_sigma](Time, State s) { return _sigma * s; });
//}
//// Theoretical solution for GBM:
//// f(s)=\frac{1}{s \sqrt{2 \pi \sigma^2 T}} \exp \left[-\frac{\left(\ln \left(\frac{s}{S_0}\right)-\left(\mu-\frac{\sigma^2}{2}\right) T\right)^2}{2 \sigma^2 T}\right]
//static inline auto GetPDF(const State startValue, const Time time, const double _mu, const double _sigma) -> PDF {

//    const double EV = startValue * std::exp(_mu * time);
//    const double stddev = EV * sqrt(exp(_sigma * _sigma * time) - 1);

//    // Pre computed constants
//    const double sigma_squared = _sigma * _sigma;
//    const double adjusted_drift = _mu - 0.5 * sigma_squared;
//    const double sqrt_2pi = std::sqrt(2.0 * PI);
//    const double sigma_t = _sigma * std::sqrt(time);
//    const double variance_t = sigma_squared * time;
//    const double two_variance_t = 2.0 * variance_t;
//    const auto _pdf = [=](const State endValue) -> Density {
//        if (endValue < 0.0) {
//            throw std::invalid_argument("GBM cannot take negative value");
//        }
//        const double log_ratio = std::log(endValue / startValue);
//        const double drift_term = adjusted_drift * time;
//        const double diff = log_ratio - drift_term;
//        const double diff_squared = diff * diff;
//        const double denominator = endValue * sigma_t * sqrt_2pi;
//        const double exponent = -diff_squared / two_variance_t;
//        return std::exp(exponent) / denominator;
//        };
//    return PDF(EV, stddev, _pdf);
//}