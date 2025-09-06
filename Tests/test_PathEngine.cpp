#include <gtest/gtest.h>
#include "PathEngine.hpp"

class EngineThreadPoolTest : public ::testing::Test {
protected:
    static constexpr double absoluteDoubleTolerance = 1e-9;
};

class DefinitionParametersTest : public EngineThreadPoolTest {};

class SimulationParametersTest : public EngineThreadPoolTest {};

class SettingsParametersTest : public EngineThreadPoolTest {};


TEST_F(EngineThreadPoolTest, basicDriftTest) {
    double driftCoefficient = 0.1;
    DeterministicQuery query{};
    query.definitionParameters.drift = getField(FieldTags::drift{}, ProcessType::BM, driftCoefficient);
    PathEngine engine{};
    auto deterministicJobVariant = engine.processQuery(query);
    ASSERT_TRUE(std::holds_alternative<DeterministicJob>(deterministicJobVariant));
    auto& job = std::get<DeterministicJob>(deterministicJobVariant);
    auto drift = job.drift.get();
    ASSERT_EQ(drift.size(), 1 + query.simulationParameters.steps());
    ASSERT_DOUBLE_EQ(drift.front(), query.definitionParameters.X0);

    State theoreticalEV = getField(FieldTags::pdf{},
        query.definitionParameters.type,
        query.definitionParameters.X0,
        query.simulationParameters.time,
        query.definitionParameters.drift.mu(),
        query.definitionParameters.diffusion.sigma()).EV;

    ASSERT_DOUBLE_EQ(theoreticalEV, driftCoefficient * query.simulationParameters.time + query.definitionParameters.X0);
    ASSERT_NEAR(
        drift.back(),
        theoreticalEV,
        absoluteDoubleTolerance);
    for (size_t i = 0; i < drift.size(); ++i) {
        ASSERT_NEAR(drift[i],
            driftCoefficient * i * query.simulationParameters.dt + query.definitionParameters.X0,
            absoluteDoubleTolerance);
    }
}

TEST_F(DefinitionParametersTest, deterministicSolverTest){
    double driftCoefficient = 0.2;
    ProcessType processType = ProcessType::GBM;
    PathEngine engine{};
    DeterministicQuery queryEM{processType};
    queryEM.simulationParameters.solver = SolverType::EULER_MARUYAMA;
    State theoreticalEV = getField(FieldTags::pdf{},
        queryEM.definitionParameters.type,
        queryEM.definitionParameters.X0,
        queryEM.simulationParameters.time,
        queryEM.definitionParameters.drift.mu(),
        queryEM.definitionParameters.diffusion.sigma()).EV;

    auto queryRK = queryEM;
    auto queryMIL = queryEM;
    queryRK.simulationParameters.solver = SolverType::RUNGE_KUTTA;
    queryMIL.simulationParameters.solver = SolverType::MILSTEIN;
    State EV_EM = std::get<DeterministicJob>(engine.processQuery(queryEM)).drift.get().back();
    State EV_RK = std::get<DeterministicJob>(engine.processQuery(queryRK)).drift.get().back();
    State EV_MIL = std::get<DeterministicJob>(engine.processQuery(queryMIL)).drift.get().back();
    State EV_EM_Error = std::fabs(EV_EM/theoreticalEV - 1);
    State EV_RK_Error = std::fabs(EV_RK/theoreticalEV - 1);
    State EV_MIL_Error = std::fabs(EV_MIL/theoreticalEV - 1);

    // For deterministic paths RK and MIL should match EM
    ASSERT_EQ(EV_EM_Error, EV_RK_Error);
    ASSERT_EQ(EV_EM_Error, EV_MIL_Error);

    ASSERT_NE(EV_EM_Error, 0);

    // Error is within 1%
    ASSERT_NEAR(EV_EM_Error, 0, 0.01);

    // Error decrease with smaller time steps
    queryEM.simulationParameters.dt = queryEM.simulationParameters.dt / 10;
    queryRK.simulationParameters.dt = queryRK.simulationParameters.dt / 10;
    queryMIL.simulationParameters.dt = queryMIL.simulationParameters.dt / 10;
    State EV_EM_smallStep = std::get<DeterministicJob>(engine.processQuery(queryEM)).drift.get().back();
    State EV_RK_smallStep = std::get<DeterministicJob>(engine.processQuery(queryRK)).drift.get().back();
    State EV_MIL_smallStep = std::get<DeterministicJob>(engine.processQuery(queryMIL)).drift.get().back();
    State EV_EM_Error_smallStep = std::fabs(EV_EM_smallStep/theoreticalEV - 1);
    State EV_RK_Error_smallStep = std::fabs(EV_RK_smallStep/theoreticalEV - 1);
    State EV_MIL_Error_smallStep = std::fabs(EV_MIL_smallStep/theoreticalEV - 1);
    ASSERT_EQ(EV_EM_Error_smallStep, EV_RK_Error_smallStep);
    ASSERT_EQ(EV_EM_Error_smallStep, EV_MIL_Error_smallStep);

    // Closer to true drift line
    ASSERT_GT(EV_EM_Error, EV_EM_Error_smallStep);
}

TEST_F(DefinitionParametersTest, solversDifferentOnSamePathTest){
    double diffusionCoefficient = 0.3;
    size_t seed = 42;
    ProcessType processType = ProcessType::GBM;
    PathEngine engine{};
    StochasticQuery queryEM{processType};
    queryEM.definitionParameters.diffusion = getField(FieldTags::diffusion{}, processType, diffusionCoefficient);
    queryEM.simulationParameters.solver = SolverType::EULER_MARUYAMA;
    queryEM.settingsParameters.seed = seed;
    auto queryRK = queryEM;
    auto queryMIL = queryEM;
    queryRK.simulationParameters.solver = SolverType::RUNGE_KUTTA;
    queryMIL.simulationParameters.solver = SolverType::MILSTEIN;
    State XT_EM = std::get<StochasticJob>(engine.processQuery(queryEM)).distribution.get().back();
    State XT_RK = std::get<StochasticJob>(engine.processQuery(queryRK)).distribution.get().back();
    State XT_MIL = std::get<StochasticJob>(engine.processQuery(queryMIL)).distribution.get().back();

    // Check all solvers produce different results
    ASSERT_NE(XT_EM, XT_RK);
    ASSERT_NE(XT_EM, XT_MIL);
    ASSERT_NE(XT_RK, XT_MIL);

    // Check all solvers XT results are within 1%
    ASSERT_GT(0.01, XT_EM/XT_RK - 1);
    ASSERT_GT(0.01, XT_EM/XT_MIL - 1);
    ASSERT_GT(0.01, XT_RK/XT_MIL - 1);
}

TEST_F(SettingsParametersTest, seedTest) {
    size_t seed = 42;
    StochasticFullPathsQuery query1{};
    query1.settingsParameters.seed = seed;
    StochasticFullPathsQuery query2{};
    query2.settingsParameters.seed = seed;
    StochasticFullPathsQuery query3{};
    query3.settingsParameters.seed = seed + 1;
    PathEngine engine{};
    auto sJobVariant1 = engine.processQuery(query1);
    auto sJobVariant2 = engine.processQuery(query2);
    auto sJobVariant3 = engine.processQuery(query3);
    ASSERT_TRUE(std::holds_alternative<StochasticFullPathsJob>(sJobVariant1));
    ASSERT_TRUE(std::holds_alternative<StochasticFullPathsJob>(sJobVariant2));
    ASSERT_TRUE(std::holds_alternative<StochasticFullPathsJob>(sJobVariant3));
    auto& job1 = std::get<StochasticFullPathsJob>(sJobVariant1);
    auto& job2 = std::get<StochasticFullPathsJob>(sJobVariant2);
    auto& job3 = std::get<StochasticFullPathsJob>(sJobVariant3);
    Path path1 = job1.fullPaths.get().front();
    Path path2 = job2.fullPaths.get().front();
    Path path3 = job3.fullPaths.get().front();
    size_t points = path1.size();
    ASSERT_EQ(points, path2.size());
    ASSERT_EQ(points, path3.size());
    for(size_t i = 0; i < points; ++i) {
        ASSERT_DOUBLE_EQ(path1[i], path2[i]);
    }
    bool anyDifferent = false;
    for (size_t i = 0; i < path1.size(); ++i) {
        if (std::fabs(path1[i] - path3[i]) > absoluteDoubleTolerance) { anyDifferent = true; break; }
    }
    EXPECT_TRUE(anyDifferent);
}

// TEST_F(StochasticFullPathsQueryTest, singleThreadTest) {
// }


// TEST_F(, smallerTimeStepSmallerErrorTest) {
// }

// TEST_F(, smallerTimeSmallerErrorTest) {
// }

// TEST_F(, betterSolverCloserToEVTest) {
// }


