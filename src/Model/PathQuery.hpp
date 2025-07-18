#pragma once
#include "ProcessData.hpp"
#include "Constants.hpp"

struct SimulationParameters {
    explicit SimulationParameters() = default;
    explicit SimulationParameters(SolverType _solver, Time _time, Time _dt, size_t _samples)
        : solver(_solver)
        , time(_time)
        , dt(_dt)
        , samples(_samples)
    {
        assertValidParameters();
    }
    void assertValidParameters() const{
        if (dt <= 0 || time <= 0) {
            throw std::invalid_argument("Time and points must be greater than 0");
        }
    }
    [[nodiscard]] size_t points() const{
        return static_cast<size_t>(std::ceil(time / dt));
    }
    SolverType solver = DefaultConstants::Simulation::solver;
    Time time = DefaultConstants::Simulation::time;
    Time dt = DefaultConstants::Simulation::dt;
    size_t samples = DefaultConstants::Simulation::samples;
};

struct ProcessDefinition {
    constexpr auto process = DefaultConstants::process;
    ProcessType type = process;
    Drift drift = ProcessData::getDrift<process>(ProcessData::getMuData<process>().defaultValue);
    Diffusion diffusion = ProcessData::getDiffusion<process>(ProcessData::getSigmaData<process>().defaultValue);
    State startValueData = ProcessData::getStartValueData<process>().defaultValue;

    explicit ProcessDefinition() = default;
    // explicit ProcessDefinition(const ProcessType t)
    //     : type(t)
    //     , drift(ProcessData::getDrift(t, ProcessData::getMuData(t).defaultValue))
    //     , diffusion(ProcessData::getDiffusion<t>(ProcessData::getSigmaData<t>().defaultValue))
    //     , startValueData(ProcessData::getStartValueData(t).defaultValue)
    // {}

    explicit ProcessDefinition(const ProcessType t, Drift d, Diffusion diff, const State start)
        : type(t)
        , drift(d)
        , diffusion(diff)
        , startValueData(start)
    {
    }
};

struct SettingsParameters{
    bool useThreading;
    std::pair<bool, size_t> useSeed;
    explicit SettingsParameters()
    : useThreading(true)
    , useSeed({false, 0})
    {}
};

struct PathQuery {
    const ProcessDefinition processDefinition;
    const SimulationParameters simulationParameters;
    const SettingsParameters settingsParameters;
    PathQuery(const ProcessDefinition& def, const SimulationParameters& simParam, const SettingsParameters& settParam)
        : processDefinition(def)
        , simulationParameters(simParam)
        , settingsParameters(settParam)
    {
    }
};
