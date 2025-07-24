#pragma once
#include "Game.h"
#include "event/Event.h"
#include <string>

using CBEventfn_t = std::function<void(Event&)>;
struct WindowData
{
	unsigned int width;
	unsigned int height;
	std::string title;

	CBEventfn_t CBEventfn;
};

class Window
{
public:
	Window(unsigned int width, unsigned int height, std::string name)
		: m_Data(WindowData{ .width = width, .height = height, .title = name })
	{
		Init();
	}
	Window() = default;
	~Window() {}

	void SetCBEventfn(CBEventfn_t fn) { m_Data.CBEventfn = fn; }
	void Update();
	bool IsWindowClosed() const;
	void Init();
private:
	WindowData m_Data{};
	GLFWwindow* m_Window{};
};

