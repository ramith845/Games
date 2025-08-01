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
	Window(unsigned int width, unsigned int height, std::string name);
	Window() = default;
	~Window();

	bool Init();
	bool IsSuccess() const { return m_Success; }

	void SetCBEventfn(CBEventfn_t fn) { m_Data.CBEventfn = fn; }
	void SwapBuffers();
	void Viewport(int width, int height);
	void SetTitle(const char* title);

public:
private:
	WindowData m_Data{};
	bool m_Success{ true };
	GLFWwindow* m_Window{};
};

