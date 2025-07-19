#include "pch.h"
#include "GLFW/glfw3.h"
#include "Game.h"


constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height);

#ifdef NDEBUG
int WINAPI WinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
	[[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nCmdShow)
#else
int main()
#endif 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "breakout", NULL, NULL);

	if (!window)
	{
		std::cerr << "Window not created" << "\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Gladd loader failed:(";
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	std::unique_ptr<Game> game{ std::make_unique<Game>(SCR_WIDTH, SCR_HEIGHT) };

	/////////////GAME LOOP////////////////
	static float lastFrame = 0.0f;
	if (game && game->Init())
		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = static_cast<float>(glfwGetTime()); // seconds since program start
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			//std::cout << deltaTime << "\n";

			glClearColor(1.0, 1.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			//game->ProcessInput(deltaTime);
			game->Update(deltaTime);
			game->Run();

			glfwPollEvents();
			glfwSwapBuffers(window);
		}

	glfwTerminate();

	return 0;
}



void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
