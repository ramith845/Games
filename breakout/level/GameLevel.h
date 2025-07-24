#pragma once
#include "object/GameObject.h"
#include "renderer/SpriteRenderer.h"

enum class BrickType
{
	EMPTY,
	SOLID_BRICK,
	BLUE_BRICK,
	GREEN_BRICK,
	YELLOW_BRICK,
	RED_BRICK,
};

class GameLevel
{
public:
public:
	GameLevel();
	~GameLevel();
	void Draw(SpriteRenderer* renderer);
	bool IsCompleted();
	void LoadLevel(const char* file, unsigned int lvlWidth, unsigned int lvlHeight);
	void Init(std::vector<std::vector<int>> tileData, unsigned int lvlWidth, unsigned int lvlHeight);
private:
	std::vector<GameObject*> m_Entities{};
};

