#include "TaskQueue.hpp"

auto TaskQueue::Pop() -> std::function<void()>
{
}

auto TaskQueue::PushBack(std::function<void()>) -> void
{
}

auto TaskQueue::PushFront(std::function<void()>) -> void
{
}

auto TaskQueue::Empty() const -> bool
{
	return false;
}
