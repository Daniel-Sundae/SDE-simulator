#pragma once
#include "Types.hpp"
#include <QtCore/qstring.h>
#include <stdexcept>

struct SDEMetaData{
    const QString name;
    const QString acronym;
    const QString description;
    const QString definition;
    // Constructor to ensure all fields are initialized
    SDEMetaData(
        QString name,
        QString acronym,
        QString description,
        QString definition)
        : name(std::move(name)),
        acronym(std::move(acronym)),
        description(std::move(description)),
        definition(std::move(definition))
    {}
    
    static inline auto Create(SDEType type) -> SDEMetaData{
        switch(type){
            case SDEType::BM:
                return SDEMetaData(
                    QString("Brownian Motion"),
                    QString("BM"),
                    QString("Standard brownian motion. μ = 0, σ = 1"),
                    QString("dX = dB"));
            case SDEType::GBM:
                return SDEMetaData(
                    QString("Geometric Brownian Motion"),
                    QString("GBM"),
                    QString("Geometric brownian motion."),
                    QString("dX = μXdt + σXdB"));
            default:
                throw std::runtime_error("Unexpected SDE type");
        }   
    }
};

