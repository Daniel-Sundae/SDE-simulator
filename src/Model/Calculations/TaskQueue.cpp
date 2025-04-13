#include "TaskQueue.hpp"

TaskQueue::TaskQueue()
	: m_tasks()
	, m_qMtx()
{
}

auto TaskQueue::Pop() -> std::optional<Task>
{
	std::scoped_lock sl(m_qMtx);
	if (m_tasks.empty()) {
		return std::nullopt;
	}
	auto task = std::move(m_tasks.front());
	m_tasks.pop_front();
	return task;
}

auto TaskQueue::PushBack(Task&& task) -> void
{
	std::scoped_lock sl(m_qMtx);
	m_tasks.push_back(std::move(task));
}

auto TaskQueue::PushFront(Task&& task) -> void
{
	std::scoped_lock sl(m_qMtx);
	m_tasks.push_front(std::move(task));
}

auto TaskQueue::Empty() const -> bool
{
	std::scoped_lock sl(m_qMtx);
	return m_tasks.empty();
}
