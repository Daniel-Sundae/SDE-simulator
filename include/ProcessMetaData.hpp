#pragma once
#include "Types.hpp"
#include <stdexcept>

class ProcessData {
public:

    template <typename T>
    concept InfoProvided = requires {
        { T::name } -> std::same_as<const std::string_view>;
        { T::acronym } -> std::same_as<const std::string_view>;
        { T::description } -> std::same_as<const std::string_view>;
        { T::definition } -> std::same_as<const std::string_view>;
    };

    struct BM{
        static auto Drift() -> Drift {
            return [](Time, State) { return 0.0; };
        }
        static auto Diffusion() -> Drift {
            return [](Time, State) { return 1.0; };
        }

        static constexpr std::string_view name = "Brownian Motion";
        static constexpr std::string_view acronym = "BM";
        static constexpr std::string_view description = "Standard brownian motion. μ = 0, σ = 1";
        static constexpr std::string_view definition = "dX = dB";
    };

    struct GBM{
        static auto Drift(const double mu) -> Drift {
            return [mu](Time, State s) { return mu * s; };
        }
        static auto Diffusion(const double sigma) -> Drift {
            return [sigma](Time, State s) { return sigma * s; };
        }

        static constexpr std::string_view name = "Geometric Brownian Motion";
        static constexpr std::string_view acronym = "GBM";
        static constexpr std::string_view description = "Geometric brownian motion.";
        static constexpr std::string_view definition = "dX = μXdt + σXdB";
    };

    static_assert(InfoProvided<BM>);
    static_assert(InfoProvided<GBM>);
};
