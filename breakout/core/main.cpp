#include "pch.h"
#include "window.h"
#include "Game.h"

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;

#ifdef NDEBUG
int WINAPI WinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
	[[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nCmdShow)
#else
int main()
#endif 
{
	std::unique_ptr<Game> game{ std::make_unique<Game>(SCR_WIDTH, SCR_HEIGHT) };
	game->Init();
	game->Run();
	return 0;
}
