#pragma once  
#include "Window.h"  
#include "level/GameLevel.h"  
#include "object/GameObject.h"  
#include "object/BallObject.h"  
#include "object/PowerUp.h"  
#include "event/Event.h"  
#include "event/KeyEvent.h"  
#include "event/MouseEvent.h"  
#include "event/WindowEvent.h"  
#include "renderer/PostProcessor.h"
#include "renderer/SpriteRenderer.h"
#include "manager/ParticleGenerator.h"
#include "irrKlang/irrKlang.h"

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

/*struct DragObject
{
	glm::vec2 initialPos{};
	glm::vec2 centerPos{};
	glm::vec2 GetOffset()
	{
		return centerPos - initialPos;
	}
};*/

class Game
{
public:
	using IrrKlangEnginePtr_t = std::unique_ptr<irrklang::ISoundEngine, void(*)(irrklang::ISoundEngine*)>;
public:
	Game(unsigned int width, unsigned int height);
	Game(int width, int height);
	~Game();
	int Init();
	void InitPlayer();
	void ProcessInput();
	void Update();
	void Draw();
	void Run();
	friend void activate_power_up(Game& game, PowerUp& powerup);
	friend void load_all_levels(Game& game);
	friend bool handle_brick_collision(Game& game, GameObject& box);

private:
	//Collision_t check_collision(BallObject& a, GameObject& b);
	void DoCollisions(BallObject& obj);
	void SpawnPowerups(GameObject& obj);
	template<typename T>
	void PowerupDeactivate(T& powerup);

	void OnEvent(Event& e);
	bool OnKeyPress(KeyPressEvent& ke);
	bool OnKeyRelease(KeyReleaseEvent& kr);
	bool OnMouseMove(MouseMovedEvent& e);
	bool OnMouseBtnRelease(MouseBtnReleasedEvent& e);
	bool OnMouseBtnPress(MouseBtnPressedEvent& e);
	bool OnCloseGame(WindowCloseEvent& e);
	bool OnResizeGame(WindowResizeEvent& e);
	void ResetPlayer();
	void ResetLevel();
	void UpdatePowerUps();
	void UpdateDeltaTime();

public:
	std::unique_ptr<SpriteRenderer> renderer;
	std::unique_ptr<GameObject> Player;
	std::unique_ptr<BallObject> Ball;
	std::unique_ptr<ParticleGenerator> Particles;
	std::unique_ptr<PostProcessor> Effects;

private:
	unsigned int m_Width, m_Height;
	std::unique_ptr<Window> m_Window; // Corrected declaration to use a pointer  
	int m_Lives{ 100 };
	bool m_Keys[1024];
	float m_DeltaTime{};

	GameState m_State;

	unsigned int m_Level{};
	std::vector<std::unique_ptr<GameLevel>>  m_Levels;
	std::vector<std::unique_ptr<PowerUp>>  m_PowerUps;

	std::unique_ptr<GameObject> m_InitPlayer;
	std::unique_ptr<BallObject> m_InitBall;

	IrrKlangEnginePtr_t m_SoundEngine{ nullptr, nullptr };
	//bool m_IsDragging{ false };
	glm::vec2 m_Mouse{};
};
