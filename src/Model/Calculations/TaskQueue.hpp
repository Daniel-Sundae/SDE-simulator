#pragma once
#include <deque>
#include "Types.hpp"
#include "PathQuery.hpp"
#include <mutex>
#include <future>

// Thread safe priority based queue implemented with deque
class TaskQueue {
public:
	explicit TaskQueue();
	[[nodiscard]] auto Pop() -> std::optional<Task>;
	auto PushBack(Task&& task) -> void;
	auto PushFront(Task&& task) -> void;
	auto Empty() const -> bool;
private:
	std::deque<Task> m_tasks;
	mutable std::mutex m_qMutex;
};