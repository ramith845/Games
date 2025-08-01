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
	EVENT_CLASS_TYPE(KeyPress)
private:
	int m_Count{};
};

class KeyReleaseEvent : public KeyEvent
{
public:
	KeyReleaseEvent(int k) :KeyEvent(k) {}
	~KeyReleaseEvent() {}

	EVENT_CLASS_TYPE(KeyRelease)
};