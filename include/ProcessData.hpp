#pragma once
#include "Types.hpp"
//#include "BasicTypes.hpp"
#include <stdexcept>
#include <concepts>
#include <string_view>


class ProcessData {
public:
    using DriftFunc = Drift;
    using DiffusionFunc = Diffusion;

    struct Constants {
        const Range allowedValues;
        const double defaultValue;
        const double incrementSize;
    };

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
        static auto Drift() -> DriftFunc {
            return DriftFunc(0, [](Time, State) -> StateDot { return 0.0; });
        }
        static auto Diffusion(const double _sigma) -> DiffusionFunc {
            return DiffusionFunc(_sigma, [_sigma](Time, State) { return _sigma; });
        }

        static constexpr std::string_view name = "Brownian Motion";
        static constexpr std::string_view acronym = "BM";
        static constexpr std::string_view description = "Standard brownian motion.";
        static constexpr std::string_view definition = "dX = σdB";
        static constexpr Constants muData{ {0,0},0,0.1 };
        static constexpr Constants sigmaData{ {0,1.2},0.2,0.1 };
        static constexpr Constants startValueData{ {-100,100},0,1 };
    };

    struct GBM : RequiredFields<GBM> {
        static auto Drift(const double _mu) -> DriftFunc {
            return DriftFunc(_mu, [_mu](Time, State s) { return _mu * s; });
        }
        static auto Diffusion(const double _sigma) -> DiffusionFunc {
            return DiffusionFunc(_sigma, [_sigma](Time, State s) { return _sigma * s; });
        }

        static constexpr std::string_view name = "Geometric Brownian Motion";
        static constexpr std::string_view acronym = "GBM";
        static constexpr std::string_view description = "Geometric brownian motion.";
        static constexpr std::string_view definition = "dX = μXdt + σXdB";
        static constexpr Constants muData{ {-0.5,0.5},0,0.1 };
        static constexpr Constants sigmaData{ {0,1},0.2,0.1 };
        static constexpr Constants startValueData{ {0.1,100},1,1 };
    };

    static auto GetDrift(ProcessType processType, const double _mu) -> DriftFunc
    {
        switch (processType) 
        {
            case ProcessType::NONE:
                throw std::invalid_argument("Not implemented yet");
                break;
            case ProcessType::CUSTOM:
                throw std::invalid_argument("Not implemented yet");
            case ProcessType::BM:
                return BM::Drift();
                break;
            case ProcessType::GBM:
                return GBM::Drift(_mu);
                break;
            default:
                throw std::invalid_argument("Not implemented yet");
            }
    }

    static auto GetDiffusion(ProcessType processType, const double _sigma) -> DiffusionFunc
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
            return GBM::Diffusion(_sigma);
            break;
        default:
            throw std::invalid_argument("Not implemented yet");
        }
    }

private:
    template <auto Field>
    static auto GetField(ProcessType type) {
        switch (type) {
        case ProcessType::BM:
            return Field(ProcessData::BM{});
        case ProcessType::GBM:
            return Field(ProcessData::GBM{});
        case ProcessType::NONE:
        case ProcessType::CUSTOM:
        case ProcessType::Levy:
        case ProcessType::OU:
            throw std::runtime_error("Process type not implemented");
        }
        throw std::runtime_error("Unknown process type");
    }

//#define _GET_STATIC_FIELD(field) [](auto t) { return decltype(t)::field; }
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
#undef GET_STATIC_FIELD

};