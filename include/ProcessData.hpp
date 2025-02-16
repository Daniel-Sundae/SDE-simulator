#pragma once
#include "Types.hpp"
#include <stdexcept>
#include <concepts>
#include <string_view>


class ProcessData {
public:

    template <typename ProcessT>
    struct RequiredFields {
        constexpr RequiredFields(){
            static_assert(std::is_same_v <decltype(ProcessT::name), const std::string_view>);
            static_assert(std::is_same_v <decltype(ProcessT::acronym), const std::string_view>);
            static_assert(std::is_same_v <decltype(ProcessT::description), const std::string_view>);
            static_assert(std::is_same_v <decltype(ProcessT::definition), const std::string_view>);
        }
    };

    struct BM : RequiredFields<BM>{
        static auto Drift() -> Drift {
            return [](Time, State) { return 0.0; };
        }
        static auto Diffusion() -> Diffusion {
            return [](Time, State) { return 1.0; };
        }

        static constexpr std::string_view name = "Brownian Motion";
        static constexpr std::string_view acronym = "BM";
        static constexpr std::string_view description = "Standard brownian motion. μ = 0, σ = 1";
        static constexpr std::string_view definition = "dX = dB";
    };

    struct GBM : RequiredFields<BM> {
        static auto Drift(const double mu) -> Drift {
            return [mu](Time, State s) { return mu * s; };
        }
        static auto Diffusion(const double sigma) -> Diffusion {
            return [sigma](Time, State s) { return sigma * s; };
        }

        static constexpr std::string_view name = "Geometric Brownian Motion";
        static constexpr std::string_view acronym = "GBM";
        static constexpr std::string_view description = "Geometric brownian motion.";
        static constexpr std::string_view definition = "dX = μXdt + σXdB";
    };


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

#define _GET_STATIC_FIELD(field) [](auto t) { return decltype(t)::field; }
#define GET_STATIC_FIELD(field) ProcessData::GetField<_GET_STATIC_FIELD(field)>(type)
public:
    static auto Name(ProcessType type) -> std::string_view
    {
        return GET_STATIC_FIELD(name);
    }
    static auto Acronym(ProcessType type) -> std::string_view
    {
        return GET_STATIC_FIELD(acronym);
    }
    static auto Description(ProcessType type) -> std::string_view
    {
        return GET_STATIC_FIELD(description);
    }
    static auto Definition(ProcessType type) -> std::string_view
    {
        return GET_STATIC_FIELD(definition);
    }
#undef GET_STATIC_FIELD

};