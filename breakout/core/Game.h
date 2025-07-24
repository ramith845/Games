#pragma once  
#include "Window.h"  
#include "level/GameLevel.h"  
#include "event/Event.h"  
#include "event/KeyEvent.h"  
#include <memory>  

enum class GameState
{
	ACTIVE,
	WIN,
	MENU,
};

class Window;

class Game
{
public:
	Game(unsigned int width, unsigned int height);
	Game(int width, int height);
	~Game();
	int Init();
	void OnEvent(Event& e);
	bool OnKeyPress(KeyPressEvent& ke);
	bool OnKeyRelease(KeyReleaseEvent& kr);
	void ProcessInput();
	void Update();
	void Run();
private:
	std::unique_ptr<Window> m_Window; // Corrected declaration to use a pointer  
	GameState m_State;
	unsigned int m_Level{};
	std::vector<std::unique_ptr<GameLevel>>  m_Levels;
	unsigned int m_Width, m_Height;
	bool m_Keys[1024];
	float m_DeltaTime{};
};
