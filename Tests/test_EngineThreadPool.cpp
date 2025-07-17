#include <gtest/gtest.h>
#include "TestUtil.hpp"
#include "EngineThreadPool.hpp"
#include <chrono>
#include <mutex>
#include <latch>
#include <algorithm>

class EngineThreadPoolTest : public ::testing::Test {
protected:
    std::unique_ptr<EngineThreadPool> m_tp;
    Paths m_results;
    std::unique_ptr<std::latch> m_nrTasksLeft;

    void RuntimeSetUp(size_t nrThreads, size_t nrTasks) {
        TearDown();
        m_results.resize(nrTasks);
        m_nrTasksLeft = std::make_unique<std::latch>(nrTasks);
        m_tp = std::make_unique<EngineThreadPool>(nrThreads);
    }

    void TearDown() {
        m_nrTasksLeft.reset();
        m_tp.reset();
        m_results.clear();
    }

    std::function<void()> generateTaskFunction(const size_t taskTimeMs, const State resultState, const size_t slot){
        return [this, taskTimeMs, resultState, slot]() -> void {
            std::this_thread::sleep_for(std::chrono::milliseconds(taskTimeMs));
            m_results[slot] = { resultState };
            m_nrTasksLeft->count_down();
        };
    }
};

TEST_F(EngineThreadPoolTest, TestTpSimpleTasks) {
    size_t nrTasks = 4;
    RuntimeSetUp(1, nrTasks);

    for(size_t slot = 0; slot < nrTasks; ++slot){
        m_tp->enqueue(generateTaskFunction(GetRandomInt(1, 300), 0.1 + slot, slot));
    }
    m_nrTasksLeft->wait();

    std::vector<Path> expected = { {0.1}, {1.1}, {2.1}, {3.1} };
    ASSERT_EQ(expected, m_results);
}

TEST_F(EngineThreadPoolTest, TestTpMultiThreadIsFaster) {
    size_t maxThreads = 4; // Increase or decrease depending on machine. Higher nr means more flaky test.
    size_t nrTasks = 20;
    using Duration = std::chrono::steady_clock::duration;
    std::vector<Duration> durations = {};
    durations.reserve(maxThreads);
    // Start from nrThreads = 2 since threadpool impl requires it 
    for (size_t nrThreads = 2; nrThreads <= maxThreads; ++nrThreads) {
        RuntimeSetUp(nrThreads, nrTasks);
        auto startTime = std::chrono::steady_clock::now();
        for(size_t slot = 0; slot < nrTasks; ++slot){
            m_tp->enqueue(generateTaskFunction(50, 1.0, slot));
        }
        m_nrTasksLeft->wait();
        durations.push_back(std::chrono::steady_clock::now() - startTime);
        ASSERT_EQ(m_results, Paths(nrTasks, {1})); // Sanity check that sampling was done.
    }
    ASSERT_TRUE(std::is_sorted(durations.begin(), durations.end(), std::greater<Duration>()));
}

TEST_F(EngineThreadPoolTest, TestTpClearTasks) {
    size_t taskTime = 100;
    size_t nrTasks = 5;
    RuntimeSetUp(2, nrTasks);
    // Override latch so we only wait for the two enqueue tasks
    m_nrTasksLeft = std::make_unique<std::latch>(2);
    m_tp->enqueue(generateTaskFunction(taskTime, 0.1, 3));
    m_tp->enqueue(generateTaskFunction(taskTime, 0.2, 4));
    // Ensures threads picks up task before we clear remaining tasks
    std::this_thread::sleep_for(std::chrono::milliseconds(taskTime/10));
    ASSERT_EQ(m_tp->nrBusyThreads(), 2);
    m_tp->clearTasks();
    m_nrTasksLeft->wait(); // Only waits for 2 working threads
    // Ensures threadpool threads have decremented nrBusyThreads counter
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    ASSERT_EQ(m_tp->nrBusyThreads(), 0);
    ASSERT_EQ(m_results, Paths({{}, {}, {}, {0.1}, {0.2}}));
}
