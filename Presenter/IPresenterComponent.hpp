#pragma once

template <typename ListenerType>
class IPresenterComponent{
	virtual ~IPresenterComponent() = default;
public:
	auto SetListener(const ListenerType* listener) -> void
	{ m_listener = listener; };
	auto Listener() const -> const ListenerType*
	{ return m_listener; }
private:
	const ListenerType* m_listener;
};