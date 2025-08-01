#include "pch.h"
#include "GameObject.h"
#include "utils/Random.h"

unsigned int s_IdCounter{ 0 };

GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2D* texture,
	bool solid, glm::vec3 color, glm::vec2 velocity, float rotation)
	: m_Position(position), m_Size(size), m_Velocity(velocity)
	, m_Rotation(rotation), m_Texture(texture), m_Color(color)
	, m_bb(BoundinBox(glm::vec2{ position }, glm::vec2{ position + size }))
	, m_Alive(true), m_Solid(solid)
{
	std::string tmp = std::format("{}{}_{}{}{}_{}_{}{}",
		position.x, position.y,
		color.x, color.g, color.b,
		Random::get(1.f, 100.f),
		texture->GetID(), s_IdCounter++);
	//m_ID = tmp.c_str();

	m_ID = new char[tmp.size() + 1];
	errno_t result = strcpy_s(const_cast<char*>(m_ID), tmp.size() + 1, tmp.c_str());
	if (result != 0) {
		std::cerr << "[GameObject][ERROR] Failed to copy ID\n";
	}
}


GameObject::~GameObject()
{
	std::println("[GameObject] Cleaning and deleting resources...");
	delete[] m_ID;
	std::println("[GameObject] Cleaned up.");
}

void GameObject::Render(SpriteRenderer* renderer)
{
	renderer->DrawSprite(m_Texture, m_Position, m_Rotation, m_Size, m_Color);
}

const char* GameObject::GetID() const
{
	return m_ID;
}

void GameObject::ResetSidCounter()
{
	s_IdCounter = 0;
}

glm::vec2 GameObject::GetCenter()
{
	return glm::vec2{ m_Position.x + m_Size.x / 2, m_Position.y + m_Size.y / 2 };
}