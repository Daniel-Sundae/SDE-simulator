#include <gtest/gtest.h>
#include "PathEngine.hpp"

class PathEngineTest : public ::testing::Test {
protected:
    static constexpr double relativeSimulationTolerance = 1e-3;
    static constexpr size_t seed = 42;
    State theoreticalEV(const PathQuery& pq) {
        return getField(FieldTags::pdf{},
            pq.definitionParameters.type,
            pq.definitionParameters.X0,
            pq.simulationParameters.time,
            pq.definitionParameters.drift.mu(),
            pq.definitionParameters.diffusion.sigma()).EV;
    }
    bool pathsEqual(const Path& p1, const Path& p2) {
        if (p1.size() != p2.size()) return false;
        for (size_t i = 0; i < p1.size(); ++i) {
            if (p1[i] != p2[i]) return false;
        }
        return true;
    }
};

class DefinitionParametersTest : public PathEngineTest {};

class SimulationParametersTest : public PathEngineTest {};

class SettingsParametersTest : public PathEngineTest {};

TEST_F(PathEngineTest, deterministicJobTest) {
    PathEngine engine{};
    ProcessType pt = ProcessType::GBM;
    State X0 = getField(FieldTags::X0{}, pt).defaultValue;
    DeterministicQuery query{pt};
    State theorEV = theoreticalEV(query);
    auto deterministicJobVariant = engine.processQuery(query);
    ASSERT_TRUE(std::holds_alternative<DeterministicJob>(deterministicJobVariant));
    auto& job = std::get<DeterministicJob>(deterministicJobVariant);
    job.drift.wait();
    Job::MetaData& data = *(job.metaData);
    size_t pathsCompleted = data.pathsCompleted.load();
    State minXT = data.minXT.load();
    State maxXT = data.maxXT.load();
    State minXt = data.minXt.load();
    State maxXt = data.maxXt.load();
    ASSERT_EQ(pathsCompleted, query.simulationParameters.samples);
    ASSERT_DOUBLE_EQ(minXT, maxXt);
    ASSERT_DOUBLE_EQ(minXT, maxXT);
    ASSERT_NEAR(minXT/theorEV, 1, relativeSimulationTolerance);
    ASSERT_DOUBLE_EQ(minXt, X0);
}

TEST_F(PathEngineTest, stochasticJobTest) {
    PathEngine engine{};
    ProcessType pt = ProcessType::OU;
    State X0 = getField(FieldTags::X0{}, pt).defaultValue;
    StochasticQuery query{pt};
    query.simulationParameters.samples = 10;
    auto stochasticJobVariant = engine.processQuery(query);
    ASSERT_TRUE(std::holds_alternative<StochasticJob>(stochasticJobVariant));
    auto& job = std::get<StochasticJob>(stochasticJobVariant);
    job.distribution.wait();
    Job::MetaData& data = *(job.metaData);
    size_t pathsCompleted = data.pathsCompleted.load();
    State minXT = data.minXT.load();
    State maxXT = data.maxXT.load();
    State minXt = data.minXt.load();
    State maxXt = data.maxXt.load();
    ASSERT_EQ(pathsCompleted, query.simulationParameters.samples);
    // [minXT, maxXT] subset of [minXt, maxXt]
    ASSERT_LE(minXt, minXT);
    ASSERT_GE(maxXt, maxXT);
    ASSERT_LE(minXt, X0);
    ASSERT_GE(maxXt, X0);
}

TEST_F(PathEngineTest, cancelSinglePathTest){
    PathEngine engine{};
    DeterministicQuery query{};
    query.simulationParameters.time = 1000;
    auto job = std::get<DeterministicJob>(engine.processQuery(query));
    job.stop.request_stop();
    job.drift.wait();
    ASSERT_EQ(job.metaData->pathsCompleted.load(), 0);
}

TEST_F(PathEngineTest, cancelMultiplePathsTest){
    PathEngine engine{};
    size_t samples = 100000;
    StochasticQuery query{};
    query.simulationParameters.samples = samples;
    auto job = std::get<StochasticJob>(engine.processQuery(query));
    // Let threadpool start finishing some paths
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    job.stop.request_stop();
    job.distribution.wait();
    size_t pathsCompleted = job.metaData->pathsCompleted.load();
    ASSERT_TRUE(0 < pathsCompleted);
    ASSERT_TRUE(pathsCompleted < samples);
}

TEST_F(DefinitionParametersTest, basicDriftTest) {
    PathEngine engine{};
    double driftCoefficient = 0.1;
    State X0 = 5.0;
    DeterministicQuery query{};
    query.definitionParameters.drift = getField(FieldTags::drift{}, ProcessType::BM, driftCoefficient);
    query.definitionParameters.X0 = X0;
    auto job = std::get<DeterministicJob>(engine.processQuery(query));
    auto drift = job.drift.get();
    ASSERT_EQ(drift.size(), 1 + query.simulationParameters.steps());
    ASSERT_DOUBLE_EQ(drift.front(), query.definitionParameters.X0);
    State theorEV = theoreticalEV(query);
    ASSERT_DOUBLE_EQ(theorEV, driftCoefficient * query.simulationParameters.time + query.definitionParameters.X0);
    ASSERT_NEAR(drift.back()/theorEV, 1, relativeSimulationTolerance);
    for (size_t i = 0; i < drift.size(); ++i) {
        State expected = driftCoefficient * i * query.simulationParameters.dt + query.definitionParameters.X0;
        ASSERT_NEAR(drift[i]/expected, 1, relativeSimulationTolerance);
    }
}

TEST_F(DefinitionParametersTest, stochasticNoDiffusionIsDeterministicTest) {
    PathEngine engine{};
    StochasticFullPathsQuery query{};
    DeterministicQuery detQuery(query);
    Path stochasticPath = std::get<StochasticFullPathsJob>(engine.processQuery(query)).fullPaths.get().front();
    Path deterministicPath = std::get<DeterministicJob>(engine.processQuery(detQuery)).drift.get();
    ASSERT_FALSE(pathsEqual(stochasticPath, deterministicPath));
    StochasticFullPathsQuery queryNoDiffusion{};
    queryNoDiffusion.definitionParameters.diffusion = getField(FieldTags::diffusion{}, queryNoDiffusion.definitionParameters.type, 0.0);
    Path stochasticPathNoDiffusion = std::get<StochasticFullPathsJob>(engine.processQuery(queryNoDiffusion)).fullPaths.get().front();
    ASSERT_TRUE(pathsEqual(stochasticPathNoDiffusion, deterministicPath));
}

TEST_F(SimulationParametersTest, deterministicSolverTest){
    PathEngine engine{};
    double driftCoefficient = 0.2;
    ProcessType processType = ProcessType::GBM;
    DeterministicQuery queryEM{processType};
    queryEM.simulationParameters.solver = SolverType::EULER_MARUYAMA;
    State theorEV = theoreticalEV(queryEM);
    auto queryRK = queryEM;
    auto queryMIL = queryEM;
    queryRK.simulationParameters.solver = SolverType::RUNGE_KUTTA;
    queryMIL.simulationParameters.solver = SolverType::MILSTEIN;
    State EV_EM = std::get<DeterministicJob>(engine.processQuery(queryEM)).drift.get().back();
    State EV_RK = std::get<DeterministicJob>(engine.processQuery(queryRK)).drift.get().back();
    State EV_MIL = std::get<DeterministicJob>(engine.processQuery(queryMIL)).drift.get().back();
    State EV_EM_Error = std::fabs(EV_EM/theorEV - 1);
    State EV_RK_Error = std::fabs(EV_RK/theorEV - 1);
    State EV_MIL_Error = std::fabs(EV_MIL/theorEV - 1);

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
    State EV_EM_Error_smallStep = std::fabs(EV_EM_smallStep/theorEV - 1);
    State EV_RK_Error_smallStep = std::fabs(EV_RK_smallStep/theorEV - 1);
    State EV_MIL_Error_smallStep = std::fabs(EV_MIL_smallStep/theorEV - 1);
    ASSERT_EQ(EV_EM_Error_smallStep, EV_RK_Error_smallStep);
    ASSERT_EQ(EV_EM_Error_smallStep, EV_MIL_Error_smallStep);

    // Closer to true drift line
    ASSERT_GT(EV_EM_Error, EV_EM_Error_smallStep);
}

TEST_F(SimulationParametersTest, solversDifferentOnSamePathTest){
    PathEngine engine{};
    double diffusionCoefficient = 0.3;
    ProcessType processType = ProcessType::GBM;
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

    // Solvers produce different results
    ASSERT_NE(XT_EM, XT_RK);
    ASSERT_NE(XT_EM, XT_MIL);
    ASSERT_NE(XT_RK, XT_MIL);

    // Solver results are within 1%
    ASSERT_GT(0.01, XT_EM/XT_RK - 1);
    ASSERT_GT(0.01, XT_EM/XT_MIL - 1);
    ASSERT_GT(0.01, XT_RK/XT_MIL - 1);
}

TEST_F(SettingsParametersTest, seedTest) {
    PathEngine engine{};
    StochasticFullPathsQuery query1{};
    query1.settingsParameters.seed = seed;
    StochasticFullPathsQuery query2{};
    query2.settingsParameters.seed = seed;
    StochasticFullPathsQuery query3{};
    query3.settingsParameters.seed = seed + 1;
    Path path1 = std::get<StochasticFullPathsJob>(engine.processQuery(query1)).fullPaths.get().front();
    Path path2 = std::get<StochasticFullPathsJob>(engine.processQuery(query2)).fullPaths.get().front();
    Path path3 = std::get<StochasticFullPathsJob>(engine.processQuery(query3)).fullPaths.get().front();
    EXPECT_TRUE(pathsEqual(path1, path2));
    EXPECT_FALSE(pathsEqual(path1, path3));
}

TEST_F(SettingsParametersTest, singleThreadTest) {
    PathEngine engine{};
    StochasticFullPathsQuery MTquery{};
    MTquery.settingsParameters.seed = seed;
    StochasticFullPathsQuery STquery(MTquery);
    STquery.settingsParameters.useThreading = false;
    Path MTPath = std::get<StochasticFullPathsJob>(engine.processQuery(MTquery)).fullPaths.get().front();
    Path STPath = std::get<StochasticFullPathsJob>(engine.processQuery(STquery)).fullPaths.get().front();
    EXPECT_TRUE(pathsEqual(MTPath, STPath));
}