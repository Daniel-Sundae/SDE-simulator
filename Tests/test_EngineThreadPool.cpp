#include <gtest/gtest.h>
#include "TestUtil.hpp"
#include "EngineThreadPool.hpp"
#include <chrono>
#include <mutex>
#include <latch>
#include <algorithm>

class EngineThreadPoolTest : public ::testing::Test {
protected:
    void busySpin(size_t ms) {
        const auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
        while (std::chrono::steady_clock::now() < until) { /* spin */ }
    }
};

TEST_F(EngineThreadPoolTest, SlowTaskDoesNotBlockOthers) {
    size_t workers = 2;
    EngineThreadPool tp(workers);
    size_t nrTasks = 100;
    std::latch start(1);
    std::mutex m;
    std::vector<size_t> completionOrder;
    completionOrder.reserve(nrTasks);
    auto makeTask = [&](size_t taskTime, size_t id) {
        return [&, taskTime, id]() -> Path {
            start.wait();
            busySpin(taskTime);
            {
                std::scoped_lock lock(m);
                completionOrder.push_back(id);
            }
            return {}; // Not used
        };
    };

    std::vector<std::future<Path>> futures;
    futures.reserve(nrTasks);
    for (size_t i = 0; i < nrTasks; ++i) {
        size_t time = 0;
        // First and last tasks are slow
        if(i == 0) time = 100;
        if(i == nrTasks - 1) time = 120;
        futures.push_back(tp.enqueue(makeTask(time, i)));
    }

    start.count_down();
    for (auto& fut : futures) {
        fut.get();
    }

    ASSERT_EQ(completionOrder.size(), nrTasks);
    for (size_t i = 0; i < nrTasks-2; ++i) {
        ASSERT_EQ(completionOrder[i], i+1);
    }
    ASSERT_EQ(completionOrder[nrTasks - 2], 0); // First task should be second to last
    ASSERT_EQ(completionOrder[nrTasks - 1], nrTasks - 1); // Last task should be last
}

// TEST_F(EngineThreadPoolTest, TestTpMultiThreadIsFaster) {
//     size_t maxThreads = 4; // Increase or decrease depending on machine. Higher nr means more flaky test.
//     size_t nrTasks = 20;
//     using Duration = std::chrono::steady_clock::duration;
//     std::vector<Duration> durations = {};
//     durations.reserve(maxThreads);
//     // Start from nrThreads = 2 since threadpool impl requires it 
//     for (size_t nrThreads = 2; nrThreads <= maxThreads; ++nrThreads) {
//         RuntimeSetUp(nrThreads, nrTasks);
//         auto startTime = std::chrono::steady_clock::now();
//         for(size_t slot = 0; slot < nrTasks; ++slot){
//             m_tp->enqueue(generateTaskFunction(50, 1.0, slot));
//         }
//         m_nrTasksLeft->wait();
//         durations.push_back(std::chrono::steady_clock::now() - startTime);
//         ASSERT_EQ(m_results, Paths(nrTasks, {1})); // Sanity check that sampling was done.
//     }
//     ASSERT_TRUE(std::is_sorted(durations.begin(), durations.end(), std::greater<Duration>()));
// }

// TEST_F(EngineThreadPoolTest, TestTpClearTasks) {
//     size_t taskTime = 100;
//     size_t nrTasks = 5;
//     RuntimeSetUp(2, nrTasks);
//     // Override latch so we only wait for the two enqueue tasks
//     m_nrTasksLeft = std::make_unique<std::latch>(2);
//     m_tp->enqueue(generateTaskFunction(taskTime, 0.1, 3));
//     m_tp->enqueue(generateTaskFunction(taskTime, 0.2, 4));
//     // Ensures threads picks up task before we clear remaining tasks
//     std::this_thread::sleep_for(std::chrono::milliseconds(taskTime/10));
//     ASSERT_EQ(m_tp->nrBusyThreads(), 2);
//     m_tp->clearTasks();
//     m_nrTasksLeft->wait(); // Only waits for 2 working threads
//     // Ensures threadpool threads have decremented nrBusyThreads counter
//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     ASSERT_EQ(m_tp->nrBusyThreads(), 0);
//     ASSERT_EQ(m_results, Paths({{}, {}, {}, {0.1}, {0.2}}));
// }
