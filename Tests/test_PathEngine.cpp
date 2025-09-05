#include <gtest/gtest.h>
#include "PathEngine.hpp"

class EngineThreadPoolTest : public ::testing::Test {
protected:

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
    ASSERT_EQ(drift.size(), simulationParameters.time / simulationParameters.dt);
    ASSERT_DOUBLE_EQ(drift.front(), query.processDefinition.startValue);
    ASSERT_DOUBLE_EQ(drift.back(), driftCoefficient * query.simulationParameters.time + query.processDefinition.startValue);
    for (size_t i = 0; i < drift.size(); ++i) {
        ASSERT_DOUBLE_EQ(drift[i], driftCoefficient * i * query.simulationParameters.dt + query.processDefinition.startValue);
    }
}

// TEST_F(DeterministicQueryTest, simpleDriftTest) {
//     PathQuery query{
//         ProcessDefinition{
//             ProcessType::GeometricBrownianMotion,
//             0.1,
//             0.2,
//             100.0
//         },
//         SimulationParameters{
//             SolverType::EulerMaruyama,
//             1.0,
//             0.01,
//             10
//         },
//         SettingsParameters{}
//     };
//     PathEngine engine{};
//     auto jobVariant = engine.processQuery(query);
//     ASSERT_TRUE(std::holds_alternative<DeterministicJob>(jobVariant));
//     auto& job = std::get<DeterministicJob>(jobVariant);
//     auto drift = job.drift.get();
//     ASSERT_EQ(drift.size(), query.simulationParameters.points());
//     ASSERT_DOUBLE_EQ(drift.front(), query.processDefinition.startValue);
//     for (size_t i = 1; i < drift.size(); ++i) {
//         ASSERT_GT(drift[i], drift[i - 1]);
//     }
// }