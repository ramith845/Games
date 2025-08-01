#pragma once
#include "Event.h"

class WindowResizeEvent :
	public Event
{
public:
	WindowResizeEvent(int width, int height)
	{
		m_Width = static_cast<unsigned int>(width);
		m_Height = static_cast<unsigned int>(height);
	}
	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }
	EVENT_CLASS_TYPE(WindowResize)
private:
	unsigned int m_Width{};
	unsigned int m_Height{};
};

class WindowCloseEvent :
	public Event
{
public:
	WindowCloseEvent() {}
	EVENT_CLASS_TYPE(WindowClose)
private:
};
