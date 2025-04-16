#include <gtest/gtest.h>
#include "TestUtil.hpp"
#include "EngineThreadPool.hpp"
#include <chrono>
#include <mutex>

class EngineThreadPoolTest : public ::testing::Test {
protected:
    std::unique_ptr<EngineThreadPool> m_tp;
    Paths m_results;
    std::mutex m_resultMtx;
    std::vector< std::future<Path> > m_futures;

    void SetUp() override {
        SetUp(0);
    }

    void SetUp(unsigned int nrThreads) {
        TearDown();
        m_tp = std::make_unique<EngineThreadPool>(nrThreads);
    }

    void TearDown() {
        WaitForResults();
        m_tp.reset();
        {
           std::unique_lock lock(m_resultMtx, std::try_to_lock);
           ASSERT_TRUE(lock.owns_lock())
            << "No thread should be writing to results when tearing down.";
            m_results.clear();
        }
        m_futures.clear();
    }

    auto GenerateTaskFunction(const unsigned int taskTimeMs, const State resultState) -> std::function<Path()>
    {
        return [this, taskTimeMs, resultState]() -> Path {
            std::this_thread::sleep_for(std::chrono::milliseconds(taskTimeMs));
            {
                std::scoped_lock sl(m_resultMtx);
                m_results.push_back({ resultState });
            }
            // This returned empty path is completely ignored by tests.
            // Needed for EngineThreadPool::Enqueue interface.
            return {};
        };
    }

    auto DoEnqueue(
        std::function<Path()> f,
        std::size_t nrTasks = 1) -> void
    {
        for (std::size_t i = 0; i < nrTasks; ++i) {
            m_futures.emplace_back(m_tp->Enqueue(f));
        }
    }

    auto WaitForResults() -> void
    {
        try{
            for (auto& future : m_futures) {
                future.get();
            }
            m_futures.clear();
        }
        catch (...) {
            try {
                throw; // rethrow the current exception
            } catch (const std::exception& e) {
                std::cout << "Caught std::exception: " << e.what() << std::endl;
                // You can inspect e here
            } catch (...) {
                std::cout << "Caught unknown exception" << std::endl;
            }
        }
    };

};

TEST_F(EngineThreadPoolTest, TestTpSimpleTasks) {
    SetUp(1);
    auto task1 = GenerateTaskFunction(GetRandomInt(1, 300), 0.1);
    auto task2 = GenerateTaskFunction(GetRandomInt(1, 300), 0.2);
    auto task3 = GenerateTaskFunction(GetRandomInt(1, 300), 0.3);
    auto task4 = GenerateTaskFunction(GetRandomInt(1, 300), 0.4);

    DoEnqueue(task1);
    DoEnqueue(task2);
    DoEnqueue(task3);
    DoEnqueue(task4);
    WaitForResults();

    std::vector<Path> expected = { {0.1}, {0.2}, {0.3}, {0.4} };
    ASSERT_EQ(expected, m_results);
}

TEST_F(EngineThreadPoolTest, TestTpOrder) {
    SetUp(2);
    auto slowTask = GenerateTaskFunction(200, 0.2);
    auto fastTask = GenerateTaskFunction(0, 0.1);
    DoEnqueue(slowTask);
    DoEnqueue(fastTask);
    WaitForResults();

    // Expect that fast task is done first since there are two threads working on them simultaneously
    std::vector<Path> expected = { {0.1}, {0.2} };
    ASSERT_EQ(expected, m_results);

    SetUp(1);
    DoEnqueue(slowTask);
    DoEnqueue(fastTask);
    WaitForResults();

    // Expect slow task is done first since it was enqueued first and only one working thread
    expected = { {0.2}, {0.1} };
    ASSERT_EQ(expected, m_results);
}

TEST_F(EngineThreadPoolTest, TestTpMultiThreadIsFaster) {
    unsigned int maxThreads = 4; // Increase or decrease depending on machine. Higher nr means more flaky test.
    unsigned int nrTasks = 20;
    using Duration = std::chrono::steady_clock::duration;
    std::vector<Duration> durations = {};
    durations.reserve(maxThreads);
    auto task = GenerateTaskFunction(50, 0.1);
    for (unsigned int i = 1; i <= maxThreads; ++i) {
        SetUp(i);
        auto startTime = std::chrono::steady_clock::now();
        DoEnqueue(task, nrTasks);
        WaitForResults();
        durations.push_back(std::chrono::steady_clock::now() - startTime);
        ASSERT_EQ(m_results.size(), nrTasks);
    }
    ASSERT_TRUE(std::is_sorted(durations.begin(), durations.end(), std::greater<Duration>()));
}

TEST_F(EngineThreadPoolTest, TestTpClearTasks) {
    SetUp(1);
    unsigned int taskTime = 100;
    unsigned int nrTasks = 10;
    auto task = GenerateTaskFunction(taskTime, 0.1);
    DoEnqueue(task, nrTasks);
    // Ensures thread picks up task before we clear tasks
    std::this_thread::sleep_for(std::chrono::milliseconds(taskTime/10));
    std::cout << m_tp->NrTasks() << std::endl;
    std::cout << m_futures.size() << std::endl;
    m_tp->ClearTasks();
    std::cout << m_tp->NrTasks() << std::endl;
    std::cout << m_futures.size() << std::endl;
    WaitForResults();
    ASSERT_EQ(m_tp->NrTasks(), 0);
    ASSERT_EQ(m_results.size(), 1);
}
