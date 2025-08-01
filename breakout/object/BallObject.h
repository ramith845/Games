#pragma once
#include "GameObject.h"
class BallObject :
	public GameObject
{
public:
	BallObject();
	BallObject(glm::vec2 position, float radius, Texture2D* texture,
		glm::vec2 velocity, glm::vec3 color = glm::vec3(1.f), float rotation = 0.0f);

	void Move(float dt, unsigned int windowWidth);
	void Reset(glm::vec2 position, glm::vec2 velocity);
public:
	float m_Radius{ 0.f };
	bool m_Stuck{ true };
	bool m_Sticky{ false };
	bool m_PassThrough{ false };
};

