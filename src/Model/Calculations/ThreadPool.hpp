#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include "TaskQueue.hpp"
#include <future>
#include <thread>

class ThreadPool{
public:
	explicit ThreadPool();
	auto Enqueue(const std::function<Path(PathQuery)>& f, const PathQuery) -> std::future<Path>;
	auto Stop() -> void;
private:
	std::vector<std::jthread> m_threads;
	std::unique_ptr<TaskQueue> m_tasks;
	std::stop_source m_stopSource;
};