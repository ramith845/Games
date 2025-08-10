#pragma once
#include "Event.h"
#include <string>
#include <print>

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float x, float y) : m_X(static_cast<float>(x)), m_Y(static_cast<float>(y)) {}
	inline float GetX() const { return m_X; }
	inline float GetY() const { return m_Y; }

	virtual inline std::string ToString() const override
	{
		return std::format("Pos({:.2f}, {:.2f})", m_X, m_Y);
	}
	EVENT_CLASS_TYPE(MouseMove)
private:
	float m_X{}, m_Y{};
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(float x, float y) : m_OffsetX(x), m_OffsetY(y) {}
	inline float GetOffsetX() const { return m_OffsetX; }
	inline float GetOffsetY() const { return m_OffsetY; }

	virtual inline std::string ToString() const override
	{
		std::println("Offset({:.2f}, {:.2f})", m_OffsetX, m_OffsetY);
	}
	EVENT_CLASS_TYPE(MouseScroll)
private:
	float m_OffsetX{}, m_OffsetY{};
};

class MouseBtnEvent : public Event
{
public:
	inline int GetBtn() const { return m_Button; }

protected:
	MouseBtnEvent(int key) : m_Button(key) {}
private:
	int m_Button{};
};

class MouseBtnPressedEvent : public MouseBtnEvent
{
public:
	MouseBtnPressedEvent(int key) : MouseBtnEvent(key) {}
	virtual inline std::string ToString() const override
	{
		return std::format("Key Pressed: {}", std::to_string(GetBtn()));
	}
	EVENT_CLASS_TYPE(MouseButtonPress)
};

class MouseBtnReleasedEvent : public MouseBtnEvent
{
public:
	MouseBtnReleasedEvent(int key) : MouseBtnEvent(key) {}
	virtual inline std::string ToString() const override
	{
		return std::format("Key Released: {}", std::to_string(GetBtn()));
	}
	EVENT_CLASS_TYPE(MouseButtonRelease)
};
