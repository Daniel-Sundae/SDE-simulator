#pragma once
#include "Types.hpp"
#include <QtCore/qstring.h>
#include <stdexcept>

//struct ProcessMetaData {
//private:
//    ProcessMetaData(
//        QString in_name,
//        QString in_acronym,
//        QString in_description,
//        QString in_definition)
//        : name(std::move(in_name)),
//        acronym(std::move(in_acronym)),
//        description(std::move(in_description)),
//        definition(std::move(in_definition))
//    {}
//public:
//    const QString name;
//    const QString acronym;
//    const QString description;
//    const QString definition;
//
//    static inline auto Create(ProcessType type) -> ProcessMetaData{
//        switch(type){
//            case ProcessType::BM:
//                return ProcessMetaData(
//                    QString("Brownian Motion"),
//                    QString("BM"),
//                    QString("Standard brownian motion. μ = 0, σ = 1"),
//                    QString("dX = dB"));
//            case ProcessType::GBM:
//                return ProcessMetaData(
//                    QString("Geometric Brownian Motion"),
//                    QString("GBM"),
//                    QString("Geometric brownian motion."),
//                    QString("dX = μXdt + σXdB"));
//            default:
//                throw std::runtime_error("Unexpected Process type");
//        }   
//    }
//};


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
        static auto Drift(double mu) -> Drift {
            return [mu](Time, State s) { return mu * s; };
        }
        static auto Diffusion(double sigma) -> Drift {
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
