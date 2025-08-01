#include "pch.h"
#include "window.h"
#include "Game.h"

constexpr int SCR_WIDTH = 810;
constexpr int SCR_HEIGHT = 600;

extern "C" {
	// For NVIDIA Optimus (most laptops)
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;

	// For AMD switchable graphics (optional)
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	// GAME BLOCK
	{
		std::unique_ptr<Game> game = std::make_unique<Game>(SCR_WIDTH, SCR_HEIGHT);
		game->Init();
		game->Run();
	}
	//_CrtDumpMemoryLeaks();
	//std::cin.get();
	return 0;
}
