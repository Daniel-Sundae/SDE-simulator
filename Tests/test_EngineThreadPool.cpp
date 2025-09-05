#include <gtest/gtest.h>
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

TEST_F(EngineThreadPoolTest, simpleThreadPoolTest) {
    EngineThreadPool tp{};
    auto fut = tp.enqueue([]() -> std::optional<State> {
        return std::make_optional<State>(0);
    });
    ASSERT_TRUE(fut.valid());
    ASSERT_EQ(fut.get(), std::make_optional<State>(0));
}

TEST_F(EngineThreadPoolTest, enqueueTasks) {
    EngineThreadPool tp{};
    size_t nrTasks = 10;
    std::vector<std::future<std::optional<State>>> futPoints;
    futPoints.reserve(nrTasks);
    std::vector<std::future<Path>> futPaths;
    futPaths.reserve(nrTasks);
    auto makePointTask = [this](State state) {
        return [this, state]() -> std::optional<State> {
            busySpin(10);
            return std::make_optional<State>(state);
        };
    };
    auto makePathTask = [this](State pathState) {
        return [this, pathState]() -> Path {
            busySpin(10);
            return {pathState};
        };
    };
    for (size_t i = 0; i < nrTasks; ++i) {
        futPoints.push_back(tp.enqueue(makePointTask(static_cast<State>(i))));
        futPaths.push_back(tp.enqueue(makePathTask(static_cast<State>(i))));
    }
    for (size_t i = 0; i < nrTasks; ++i) {
        ASSERT_EQ(futPoints[i].get(), std::make_optional<State>(i));
        ASSERT_EQ(futPaths[i].get(), Path{static_cast<State>(i)});
    }
}

TEST_F(EngineThreadPoolTest, slowTaskDoesNotBlockFast) {
    size_t workers = 2;
    EngineThreadPool tp{workers};
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