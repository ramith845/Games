#pragma once
#include "Texture.h"
class GameObject
{
public:
	GameObject(int x, int y, int sizex, int sizey, float rotation, float velocity, glm::vec3 color) {}

private:
	Texture2D* m_Texture;
	glm::vec2 m_Position{ 0.f }, m_Size{ 100.f }, m_Velocity{ 0.f };
	float m_Rotation{ 0.f };
	glm::vec3 m_Color{ 1.0 };
	bool m_Alive{};
	bool m_Solid{};
};

