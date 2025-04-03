#pragma once
#include <deque>
#include "Types.hpp"
#include "PathQuery.hpp"
#include <mutex>

// Thread safe priority based queue implemented with deque
class TaskQueue {
public:
	auto Pop() -> std::function<void()>;
	auto PushBack(std::function <void()>) -> void;
	auto PushFront(std::function <void()>) -> void;
	auto Empty() const -> bool;
private:
	std::deque< std::function <void()> > m_tasks;
	std::mutex m;
};