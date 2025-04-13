#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include "TaskQueue.hpp"
#include <future>
#include <thread>

class EngineThreadPool {
public:
	explicit EngineThreadPool(unsigned int nrThreads = 0);
	~EngineThreadPool();
	auto Enqueue(const std::function<Path()> f, Priority prio) -> std::future<Path>;
	auto Stop() -> void;
private:
	// Only allow moving object to engine
	EngineThreadPool(const EngineThreadPool&) = delete;
	EngineThreadPool& operator=(const EngineThreadPool&) = delete;
	auto DoTasks(std::stop_token st) -> void;
private:
	std::vector<std::jthread> m_threads;
	std::unique_ptr<TaskQueue> m_tasks;
	std::stop_source m_stopSource;
	std::condition_variable m_cv;
	std::mutex m_cvMtx;
};