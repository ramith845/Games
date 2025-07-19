#pragma once
#include "GameObject.h"
#include "SpriteRenderer.h"

class GameLevel
{
public:
	GameLevel() {}
	~GameLevel() {}

	void LoadLevel(const char* file, int lvlWidth, int lvlHeight);
	void Init();
private:
	unsigned int m_LvlWidth, m_LvlHeight;
	std::vector<GameObject> m_Entities;
};

