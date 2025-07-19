#pragma once

enum class GameState
{
	ACTIVE,
	WIN,
	MENU,
};

class Game
{
public:
	Game(unsigned int width, unsigned int height);
	Game(int width, int height);
	~Game();
	int Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Run();
private:
	GameState m_State;
	unsigned int m_Width, m_Height;
	bool m_Keys[1024];
	glm::vec2 m_Position{};
	glm::vec2 m_Size{ 200.0f, 200.0f };
	float m_Velocity{ 0.0f };
};

