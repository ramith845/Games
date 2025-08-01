#pragma once
#include "GLFW/glfw3.h"
#include <functional>

enum EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,
	KeyPress, KeyRelease,
	MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
};
class EventDispatcher;
class Event
{
	friend EventDispatcher;
public:
	virtual EventType GetEventType() const = 0;
	virtual const char* GetEventName() const = 0;

	inline bool IsHandled() { return m_Handled; }
protected:
	bool m_Handled{ false };
};

class EventDispatcher
{
	template <typename T>
	using EventFn_t = std::function<bool(T&)>;
public:
	EventDispatcher(Event& e)
		: m_Event(e) {
	}

	template <typename T>
	inline bool Dispatch(EventFn_t<T> function)
	{
		if (m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.m_Handled = function(*dynamic_cast<T*>(&m_Event));
			return true;
		}
		return false;
	}

private:
	Event& m_Event;
};

#define EVENT_CLASS_TYPE(x) virtual EventType GetEventType() const override {return GetStaticType();} \
				 virtual const char* GetEventName() const override { return #x;} \
				 static inline EventType GetStaticType() { return EventType::##x; }