#include "pch.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include "event/KeyEvent.h"
#include "event/WindowEvent.h"
#include "manager/ResourceManager.h"


Window::Window(unsigned int width, unsigned int height, std::string name)
	: m_Data(WindowData{ .width = width, .height = height, .title = name })
{
	m_Success = Init();
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

bool Window::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_Window = glfwCreateWindow((int)m_Data.width, (int)m_Data.height, m_Data.title.c_str(), NULL, NULL);

	if (!m_Window)
	{
		std::cerr << "WindowMy not created" << "\n";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, (void*)&m_Data);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Gladd loader failed:(";
		glfwTerminate();
		return false;
	}
	std::cout << "GPU: " << glGetString(GL_RENDERER) << std::endl;

	glViewport(0, 0, (int)m_Data.width, (int)m_Data.height);
	glfwSetFramebufferSizeCallback(m_Window,
		[](GLFWwindow* window, int width, int height)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowResizeEvent e(width, height);
			data->CBEventfn(e);
		}
	);
	glfwSetKeyCallback(m_Window,
		[](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			scancode;
			mods;
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressEvent event(key, 1);
				data->CBEventfn(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleaseEvent event(key);
				data->CBEventfn(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressEvent event(key, 1);
				data->CBEventfn(event);
				break;
			}
			default:
			{
				std::println("Event type not recognized!");
				break;
			}
			}
		}
	);

	glfwSetMouseButtonCallback(m_Window,
		[](GLFWwindow* window, int button, int action, int mods)
		{
			mods;
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseBtnPressedEvent event(button);
				data->CBEventfn(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseBtnReleasedEvent event(button);
				data->CBEventfn(event);
				break;
			}
			default:
			{
				std::println("Event type not recognized!");
				break;
			}
			}
		}
	);

	glfwSetCursorPosCallback(m_Window,
		[](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseMovedEvent e((float)xpos, (float)ypos);
			data->CBEventfn(e);
		});

	glfwSetWindowCloseCallback(m_Window, 
		[](GLFWwindow* window)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent e{};
			data->CBEventfn(e);
		}
	);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void Window::SwapBuffers()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void Window::Viewport(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::SetTitle(const char* title)
{
	glfwSetWindowTitle(m_Window, title);
}

