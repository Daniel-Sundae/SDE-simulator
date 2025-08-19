#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include <queue>
#include <future>
#include <thread>
#include <expected>

class EngineThreadPool {
public:
    explicit EngineThreadPool(size_t nrThreads = 0);
    ~EngineThreadPool();
    template<typename F, typename... Args>
    std::future<Path> enqueue(F&& func, Args&&... args);
    void clearTasks();
    size_t nrBusyThreads() const;

private:
    // Only allow moving object to engine
    EngineThreadPool(const EngineThreadPool&) = delete;
    EngineThreadPool& operator=(const EngineThreadPool&) = delete;
    void doTasks();
private:
    std::vector<std::thread> m_threads{};
    std::queue<Task> m_tasks{};
    std::condition_variable m_cv{};
    std::mutex m_taskMtx{};
    std::atomic<uint32_t> m_nrBusyThreads{0};
    std::atomic<bool> m_shutdownInProgress{false};
};