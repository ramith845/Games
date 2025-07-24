#include "pch.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include "event/KeyEvent.h"


void Window::Init()
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
		//return -1;
	}
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, (void*)&m_Data);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Gladd loader failed:(";
		glfwTerminate();
		//return -1;
	}

	glViewport(0, 0, (int)m_Data.width, (int)m_Data.height);
	glfwSetFramebufferSizeCallback(m_Window, 
		[](GLFWwindow* window, int width, int height) 
		{
			window;
			glViewport(0, 0, width, height); 
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
}

bool Window::IsWindowClosed() const
{
	return glfwWindowShouldClose(m_Window);
}

void Window::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}