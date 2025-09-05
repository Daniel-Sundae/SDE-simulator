#include <gtest/gtest.h>
#include "PathEngine.hpp"

class EngineThreadPoolTest : public ::testing::Test {
protected:
    static constexpr double absoluteDoubleTolerance = 1e-9;
};

class DeterministicQueryTest : public EngineThreadPoolTest {};

class StochasticQueryTest : public EngineThreadPoolTest {};

class StochasticFullPathsQueryTest : public EngineThreadPoolTest {};

TEST_F(EngineThreadPoolTest, solverTest){

}

TEST_F(DeterministicQueryTest, simpleDriftTest) {
    double driftCoefficient = 0.1;
    auto processDefinition = ProcessDefinition{
        ProcessType::BM,
        getField(FieldTags::drift{}, ProcessType::BM, driftCoefficient),
        getField(FieldTags::diffusion{}, ProcessType::BM, 0.0),
        1
    };
    auto simulationParameters = SimulationParameters{
        SolverType::EULER_MARUYAMA,
        DefaultConstants::Simulation::time,
        DefaultConstants::Simulation::dt,
        1
    };
    auto settingsParameters = SettingsParameters{
        true,
        std::nullopt
    };
    DeterministicQuery query{
        processDefinition,
        simulationParameters,
        settingsParameters
    };
    PathEngine engine{};
    auto jobVariant = engine.processQuery(query);
    ASSERT_TRUE(std::holds_alternative<DeterministicJob>(jobVariant));
    auto& job = std::get<DeterministicJob>(jobVariant);
    auto drift = job.drift.get();
    ASSERT_EQ(drift.size(), 1 + query.simulationParameters.steps());
    ASSERT_DOUBLE_EQ(drift.front(), query.processDefinition.startValue);
    ASSERT_NEAR(
        drift.back(),
        driftCoefficient * query.simulationParameters.time + query.processDefinition.startValue,
        absoluteDoubleTolerance);
    for (size_t i = 0; i < drift.size(); ++i) {
        ASSERT_NEAR(drift[i],
            driftCoefficient * i * query.simulationParameters.dt + query.processDefinition.startValue,
            absoluteDoubleTolerance);
    }
}

TEST_F(StochasticFullPathsQueryTest, seedTest) {
    // double driftCoefficient = 0.1;
    // double diffusionCoefficient = 0.2;
    // auto processDefinition = ProcessDefinition{
    //     ProcessType::BM,
    //     getField(FieldTags::drift{}, ProcessType::BM, driftCoefficient),
    //     getField(FieldTags::diffusion{}, ProcessType::BM, diffusionCoefficient),
    //     1
    // };
    // auto simulationParameters = SimulationParameters{
    //     SolverType::EULER_MARUYAMA,
    //     DefaultConstants::Simulation::time,
    //     DefaultConstants::Simulation::dt,
    //     10
    // };
    // auto settingsParameters = SettingsParameters{
    //     false,
    //     42
    // };
    // StochasticFullPathsQuery query{
    //     processDefinition,
    //     simulationParameters,
    //     settingsParameters
    // };
    // PathEngine engine{};
    // auto jobVariant1 = engine.processQuery(query);
    // ASSERT_TRUE(std::holds_alternative<StochasticFullPathsJob>(jobVariant1));
    // auto& job1 = std::get<StochasticFullPathsJob>(jobVariant1);
    // auto fullPaths1 = job1.fullPaths.get();
    // ASSERT_EQ(fullPaths1.size(), simulationParameters.nrPathsToDraw());
    // auto jobVariant2 = engine.processQuery(query);
    // ASSERT_TRUE(std::holds_alternative<StochasticFullPathsJob>(jobVariant2));
    // auto& job2 = std::get<StochasticFullPathsJob>(jobVariant2);
    // auto fullPaths2 = job2.fullPaths.get();
    // ASSERT_EQ(fullPaths2.size(), simulationParameters.nrPathsToDraw());
    // for (size_t i = 0; i < fullPaths1.size(); ++i) {
    //     ASSERT_EQ(fullPaths1[i].size(), fullPaths2[i].size());
    //     for (size_t j = 0; j < fullPaths1[i].size(); ++j) {
    //         ASSERT_NEAR(fullPaths1[i][j], fullPaths2[i][j], m_acceptedDoubleError);
    //     }
    // }
}

TEST_F(StochasticFullPathsQueryTest, singleThreadTest) {
}