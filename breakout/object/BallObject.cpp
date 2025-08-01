#include "pch.h"
#include "BallObject.h"

BallObject::BallObject(glm::vec2 position, float radius, Texture2D* texture,
	glm::vec2 velocity, glm::vec3 color, float rotation
)
	: GameObject(position, glm::vec2(2.f * radius), texture, true, color, velocity, rotation)
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
		m_Position += m_Velocity * dt;
		if (m_Position.x <= 0.0f)
		{
			m_Velocity.x *= -1;
			m_Position.x = 0.0f;
		}
		else if (m_Position.x + m_Size.x >= windowWidth)
		{
			m_Velocity.x *= -1;
			m_Position.x = windowWidth - m_Size.x;
		}
		if (m_Position.y <= 0.0f)
		{
			m_Velocity.y *= -1;
			m_Position.y = 0.0f;
		}
		m_bb = BoundinBox(m_Position, m_Position + m_Size);
	}
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
	m_Position = position;
	m_Velocity = velocity;
}