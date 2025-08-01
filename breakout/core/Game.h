#pragma once  
#include "Window.h"  
#include "level/GameLevel.h"  
#include "object/GameObject.h"  
#include "object/BallObject.h"  
#include "object/PowerUp.h"  
#include "event/Event.h"  
#include "event/KeyEvent.h"  
#include "event/WindowEvent.h"  
#include <memory>  

enum Direction
{
	UP, RIGHT, DOWN, LEFT, END
};
using Collision_t = std::tuple<bool, Direction, glm::vec2>;

enum class GameState
{
	TERMINATE,
	WIN,
	GAMEOVER,
	ACTIVE,
	MENU
};

class Window;

class Game
{
public:
	Game(unsigned int width, unsigned int height);
	Game(int width, int height);
	~Game();
	int Init();
	void ProcessInput();
	void Update();
	void Draw();
	void Run();
private:
	Collision_t CheckCollision(BallObject& a, GameObject& b);
	void DoCollisions(BallObject& obj);
	void SpawnPowerups(GameObject& obj);
	bool AnotherPowerupActive(std::string_view type);

	void OnEvent(Event& e);
	bool OnKeyPress(KeyPressEvent& ke);
	bool OnCloseGame(WindowCloseEvent& e);
	bool OnResizeGame(WindowResizeEvent& e);
	bool OnKeyRelease(KeyReleaseEvent& kr);
	void ResetPlayer();
	void ResetLevel();
	void UpdatePowerUps();
	void UpdateDeltaTime();


private:
	unsigned int m_Width, m_Height;
	std::unique_ptr<Window> m_Window; // Corrected declaration to use a pointer  
	GameState m_State;
	std::vector<std::unique_ptr<GameLevel>>  m_Levels;
	std::vector<std::unique_ptr<PowerUp>>  m_PowerUps;
	unsigned int m_Level{};
	int m_Lives{ 3 };
	bool m_Keys[1024];
	float m_DeltaTime{};
};
