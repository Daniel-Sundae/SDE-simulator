add_executable(StochasticSimTests
    test_EngineThreadPool.cpp
)

include(GoogleTest)

target_link_libraries(StochasticSimTests
    PRIVATE
    GTest::gtest_main
    Model
)

gtest_discover_tests(StochasticSimTests)
