#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
//#include "TaskQueue.hpp"
#include <queue>
#include <future>
#include <thread>

class EngineThreadPool {
public:
	explicit EngineThreadPool(unsigned int nrThreads = 0);
	~EngineThreadPool();
	auto Enqueue(const std::function<void()> f) -> void;
	auto ClearTasks() -> void;
	auto NrTasks() const -> std::size_t;
private:
	// Only allow moving object to engine
	EngineThreadPool(const EngineThreadPool&) = delete;
	EngineThreadPool& operator=(const EngineThreadPool&) = delete;
	auto DoTasks(std::stop_token st) -> void;
private:
	std::vector<std::jthread> m_threads;
	std::queue<Task> m_tasks;
	std::stop_source m_stopSource;
	std::condition_variable m_cv;
	mutable std::mutex m_taskMtx;
};