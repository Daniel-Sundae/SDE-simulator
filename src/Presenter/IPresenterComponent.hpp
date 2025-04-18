#pragma once

template <typename ListenerType>
class IPresenterComponent{
public:
	auto SetListener(ListenerType* listener) -> void
{ m_listener = listener; };

    // Const overload: returns pointer-to-const-listener implicitly
	auto Listener() const -> ListenerType*
	{ return m_listener; }

    // Non-const overload: returns pointer-to-non-const-listener
    auto Listener() -> ListenerType*
    { return m_listener; }

protected:
	ListenerType* m_listener;
};
