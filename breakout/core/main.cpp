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


// Set breakpoints on specific allocations before any code runs
// Add this outside any function at global scope:
#ifdef _DEBUG
// Force break on specific allocation numbers
static struct AllocHook {
    AllocHook() {
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        //_CrtSetBreakAlloc(174);
        //_CrtSetBreakAlloc(175);
        //_CrtSetBreakAlloc(176);
        //_CrtSetBreakAlloc(177);
    }
} allocHook;
#endif

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

	// Try breaking on different allocation numbers that might show up in leaks

	// Take memory snapshot before game starts
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);

	// GAME BLOCK
	{
		std::unique_ptr<Game> game = std::make_unique<Game>(SCR_WIDTH, SCR_HEIGHT);
		game->Init();
	_CrtMemCheckpoint(&s2);
		game->Run();
	}

	// Take snapshot after game ends
	_CrtMemCheckpoint(&s3);

	// Show difference between snapshots
	//if (_CrtMemDifference(&s3, &s1, &s2))
		//_CrtMemDumpStatistics(&s3);

	//_CrtDumpMemoryLeaks();
	//std::cin.get();
	return 0;
}
