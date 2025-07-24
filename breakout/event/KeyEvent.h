#pragma once
#include "Event.h"

class KeyEvent : public Event
{
public:
	inline int GetKeyCode() { return m_KeyCode; }
protected:
	KeyEvent(int k) : m_KeyCode(k) {}
	int m_KeyCode{};
};

class KeyPressEvent : public KeyEvent
{
public:
	KeyPressEvent(int k, int count) :KeyEvent(k), m_Count(count) {}
	~KeyPressEvent() {}
	int GetKeyCount() { return m_Count; }
	virtual EventType GetEventType() const override
	{
		return GetStaticType();
	}
	virtual const char* GetEventName() const override
	{
		return "KeyPress";
	}
	static inline EventType GetStaticType() { return EventType::KeyPress; }
private:
	int m_Count{};
};

class KeyReleaseEvent : public KeyEvent
{
public:
	KeyReleaseEvent(int k) :KeyEvent(k) {}
	~KeyReleaseEvent() {}

	virtual EventType GetEventType() const override
	{
		return GetStaticType();
	}
	virtual const char* GetEventName() const override
	{
		return "KeyRelease";
	}
	static inline EventType GetStaticType() { return EventType::KeyRelease; }
};