#include "EngineThreadPool.hpp"

EngineThreadPool::EngineThreadPool(size_t nrThreads)
{
    nrThreads = nrThreads ? nrThreads : std::max(std::thread::hardware_concurrency(), uint32_t(1));
    m_threads.reserve(nrThreads);
    for (size_t i = 0; i < nrThreads; ++i) {
        m_threads.emplace_back([this]() {this->doTasks();});
    }
}

EngineThreadPool::~EngineThreadPool()
{
    m_shutdownInProgress = true;
    {
        std::scoped_lock lock(m_taskMtx);
        while (!m_tasks.empty()) m_tasks.pop();
        m_cv.notify_all();
    }
    for (auto& thread : m_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void EngineThreadPool::doTasks()
{
    std::move_only_function<void()> task;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(m_taskMtx);
            m_cv.wait(lock, [this]() { return !m_tasks.empty() || m_shutdownInProgress; });
            if (m_shutdownInProgress) return;
            task = std::move(m_tasks.front());
            m_tasks.pop();
        }
        task(); // Sets value on promise
    }
}