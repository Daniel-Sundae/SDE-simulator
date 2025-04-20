#pragma once

template <typename ListenerType>
class IPresenterComponent{
public:
	auto SetListener(ListenerType* listener) -> void
{ m_listener = listener; };

	auto Listener() const -> ListenerType*
	{ return m_listener; }

protected:
	ListenerType* m_listener;
};
