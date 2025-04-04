#pragma once
#include <deque>
#include "Types.hpp"
#include "PathQuery.hpp"
#include <mutex>

// Thread safe priority based queue implemented with deque
class TaskQueue {
public:
	explicit TaskQueue();
	[[nodiscard]] auto Pop() -> std::optional< std::function<void()> >;
	auto PushBack(std::function <void()>&& task) -> void;
	auto PushFront(std::function <void()>&& task) -> void;
	auto Empty() const -> bool;
private:
	std::deque< std::function <void()> > m_tasks;
	mutable std::mutex m_qMutex;
};