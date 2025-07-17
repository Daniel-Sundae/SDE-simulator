#pragma once

template <typename ListenerType>
class IPresenterComponent{
public:
    void setlistener(ListenerType* listener){ m_listener = listener; };

    ListenerType* listener() const{ return m_listener; }

protected:
    ListenerType* m_listener;
};
