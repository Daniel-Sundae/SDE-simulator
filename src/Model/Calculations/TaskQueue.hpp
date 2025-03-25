#pragma once
#include <deque>
#include "Types.hpp"
#include "PathQuery.hpp"

// Thread safe priority based queue implemented with deque
class TaskQueue {
public:
	auto Pop() -> std::function<Path(PathQuery)>;
	auto Push(TaskPriority prio) -> void;
	auto Empty() const -> bool;
private:
	std::deque< std::function<Path(PathQuery)> > m_tasks;
	std::mutex m;
};