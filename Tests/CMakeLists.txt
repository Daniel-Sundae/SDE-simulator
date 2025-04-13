add_executable(run_tests
    test_EngineThreadPool.cpp
)

target_link_libraries(run_tests
    PRIVATE
    GTest::gtest_main
    Model
    CommonIncludes
)

include(GoogleTest)
gtest_discover_tests(run_tests)
