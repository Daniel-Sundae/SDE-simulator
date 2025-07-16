#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include <queue>
#include <future>
#include <thread>

class EngineThreadPool {
public:
	explicit EngineThreadPool(std::uint32_t nrThreads = 0);
	~EngineThreadPool();
	void enqueue(const std::function<void()> f);
	void clearTasks();
    std::uint32_t nrBusyThreads() const;

private:
    // Only allow moving object to engine
	EngineThreadPool(const EngineThreadPool&) = delete;
	EngineThreadPool& operator=(const EngineThreadPool&) = delete;
	void doTasks(std::stop_token st);
private:
	std::vector<std::jthread> m_threads;
	std::queue<Task> m_tasks;
	std::stop_source m_stopSource;
	std::condition_variable m_cv;
	mutable std::mutex m_taskMtx;
	std::atomic<uint32_t> m_nrBusyThreads;
};