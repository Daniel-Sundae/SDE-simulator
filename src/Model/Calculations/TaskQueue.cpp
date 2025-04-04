#include "TaskQueue.hpp"

TaskQueue::TaskQueue()
	: m_tasks()
	, m_qMutex()
{
}

[[nodiscard]] auto TaskQueue::Pop() -> std::optional< std::function<void()> >
{
	std::scoped_lock sl(m_qMutex);
	if (m_tasks.empty()) {
		return std::nullopt;
	}
	auto task = std::move(m_tasks.front());
	m_tasks.pop_front();
	return task;
}

auto TaskQueue::PushBack(std::function<void()>&& task) -> void
{
	std::scoped_lock sl(m_qMutex);
	m_tasks.push_back(std::move(task));
}

auto TaskQueue::PushFront(std::function<void()>&& task) -> void
{
	std::scoped_lock sl(m_qMutex);
	m_tasks.push_front(std::move(task));
}

auto TaskQueue::Empty() const -> bool
{
	std::scoped_lock sl(m_qMutex);
	return m_tasks.empty();
}
