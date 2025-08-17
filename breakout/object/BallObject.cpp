#include "pch.h"
#include "BallObject.h"
#include "renderer/CollisionDebugSpriteRenderer.h"

BallObject::BallObject(glm::vec2 position, float radius, Texture2DPtr texture,
	glm::vec2 velocity, glm::vec3 color, float rotation
)
	: GameObject(position, glm::vec2(2.f * radius), texture, ObjectType::Ball, color, velocity, rotation)
	, m_Radius(radius)
{

}

BallObject::BallObject()
	: BallObject(glm::vec2(0.0f), 12.5f, nullptr, glm::vec2(0.0f))
{
}

void BallObject::Move(float dt, unsigned int windowWidth)
{
	if (!m_Stuck)
	{
		SetPos( GetPos() + m_Velocity * dt);
		if (GetPos().x <= 0.0f)
		{
			m_Velocity.x *= -1;
			SetPosX(0.0f);
		}
		else if (GetPos().x + m_Size.x >= windowWidth)
		{
			m_Velocity.x *= -1;
			SetPosX(windowWidth - m_Size.x);
		}
		if (GetPos().y <= 0.0f)
		{
			m_Velocity.y *= -1;
			SetPosY(0.0f);
		}
	}
}

// only for the purpose of debug shader setting is_ball variable
void BallObject::Render(CollisionDebugSpriteRenderer* renderer)
{
	renderer->SetBallType(GetType());
	GameObject::Render(renderer);
}

void BallObject::Render(SpriteRenderer* renderer) const
{
	GameObject::Render(renderer);
}