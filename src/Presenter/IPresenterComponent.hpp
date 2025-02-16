#pragma once

template <typename ListenerType>
class IPresenterComponent{
public:
	auto SetListener(const ListenerType* listener) -> void
	{ m_listener = listener; };
	auto Listener() const -> const ListenerType*
	{ return m_listener; }
protected:
	const ListenerType* m_listener;
};