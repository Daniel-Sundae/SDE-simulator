#pragma once

template <typename ListenerType>
class IPresenterComponent{
	virtual ~IPresenterComponent() = default;
public:
	auto SetListener(const ListenerType* listener) -> void
	{ m_listener = listener; };
private:
	const ListenerType* m_listener;
};