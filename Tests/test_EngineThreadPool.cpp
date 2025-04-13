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

    void SetUp() override {
        SetUp(0);
    }

    void SetUp(unsigned int nrThreads) {
        m_tp = std::make_unique<EngineThreadPool>(nrThreads);
    }

    void TearDown() {
        m_tp.reset();
    }

    auto GenerateTaskFunction(const unsigned int taskTimeMs, const State dummyState) -> std::function<Path()>
    {
        return [this, taskTimeMs, dummyState]() -> Path {
            std::this_thread::sleep_for(std::chrono::milliseconds(taskTimeMs));
            {
                std::scoped_lock sl(m_resultMtx);
                m_results.push_back({ dummyState });
            }
            return {}; // This path is completely ignored by tests. Needed for EngineThreadPool::Enqueue interface.
        };
    }

    auto DoEnqueue(
        std::function<Path()> f,
        std::size_t nrTasks,
        Priority prio) -> std::vector< std::future<Path> >
    {
        std::vector< std::future<Path> > futures;
        futures.reserve(nrTasks);
        for (std::size_t i = 0; i < nrTasks; ++i) {
            futures.emplace_back(m_tp->Enqueue(f, prio));
        }
        return futures;
    }

    auto WaitForResults(std::vector< std::future<Path> >& futures) -> void {
        for (auto& future : futures) {
            future.get();
        }
    };

};

TEST_F(EngineThreadPoolTest, TestTpSimpleTasks) {
    int taskDuration = GetRandomInt(100, 300);
    auto task = GenerateTaskFunction(taskDuration, 1.0);
    std::vector < std::future<Path> > futures = DoEnqueue(task, 10, Priority::HIGH);
    EngineThreadPoolTest::WaitForResults(futures);

    std::cout << "In TestTpSimpleTasks" << std::endl;
    for (auto& p : m_results) {
        std::cout << p[0];
    }
}

TEST_F(EngineThreadPoolTest, TestTpPriority) {
    std::cout << "TestTpPriority 0" << std::endl;
    TearDown();
    SetUp(1); // Use only one thread for simplicity
    int lowPrioTaskDuration = 100;
    State lowPrioPathState = 0.1;
    int highPrioTaskDuration = 0;
    State highPrioPathState = 0.2;
    auto lowPrioTask = GenerateTaskFunction(lowPrioTaskDuration, lowPrioPathState);
    auto highPrioTask = GenerateTaskFunction(highPrioTaskDuration, highPrioPathState);
    std::vector < std::future<Path> > futuresLowPrio = DoEnqueue(lowPrioTask, 20, Priority::LOW);
    std::vector < std::future<Path> > futuresHighPrio = DoEnqueue(highPrioTask, 20, Priority::HIGH);
    EngineThreadPoolTest::WaitForResults(futuresLowPrio);
    EngineThreadPoolTest::WaitForResults(futuresHighPrio);

    //Expect that all low prio tasks are first entered into the queue and that the waiting thread will start working on one of them immediately.
    //Then, the high prio tasks are entered into the front of the queue so the next tasks the thread will work on are the high prio task.
    //Finally, the thread starts finishing the low prio tasks.

    for (auto& p : m_results) {
        std::cout << p[0] << std::endl;
    }

}



TEST_F(EngineThreadPoolTest, TestTpStop) {
    GTEST_SKIP() << "TestPriorityTP not implemented yet.";
}
