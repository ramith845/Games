#pragma once
#include "GameObject.h"
#include "renderer/CollisionDebugSpriteRenderer.h"
#include "renderer/SpriteRenderer.h"

class BallObject :
	public GameObject
{
public:
	BallObject();
	BallObject(glm::vec2 position, float radius, Texture2DPtr texture,
		glm::vec2 velocity = glm::vec2(0.f), glm::vec3 color = glm::vec3(1.f), float rotation = 0.0f);
	void Render(CollisionDebugSpriteRenderer* renderer); // only for the purpose of debug shader setting is_ball variable
	void Render(SpriteRenderer* renderer) const override;
	void Move(float dt, unsigned int windowWidth);
public:
	float m_Radius{ 0.f };
	bool m_Stuck{ true };
	bool m_Sticky{ false };
	bool m_PassThrough{ false };
};

