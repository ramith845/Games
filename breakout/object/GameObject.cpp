#include "pch.h"
#include "GameObject.h"
#include "utils/Random.h"


unsigned int s_IdCounter{ 0 };

GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2DPtr texture, bool solid,
	glm::vec3 color, glm::vec2 velocity, float rotation)
	: m_Position(position), m_Size(size), m_Texture(texture), m_Color(color), m_Alive(true), m_Solid(solid), m_Velocity(velocity), m_Rotation(rotation)
{
	std::string tmp = std::format("{}{}_{}{}{}_{}_{}{}",
		position.x, position.y,
		color.x, color.g, color.b,
		Random::get(1.f, 100.f),
		texture ? texture->GetID() : 0, s_IdCounter++);

	m_ID = new char[tmp.size() + 1];
	errno_t result = strcpy_s(const_cast<char*>(m_ID), tmp.size() + 1, tmp.c_str());
	if (result != 0) {
		std::cerr << "[GameObject][ERROR] Failed to copy ID\n";
	}
	UpdateBoundingBox();
}

GameObject::GameObject(const GameObject& other)
{
	m_Position = other.m_Position;
	m_Size = other.m_Size;
	m_Velocity = other.m_Velocity;
	m_Rotation = other.m_Rotation;
	m_Texture = other.m_Texture;
	m_Color = other.m_Color;
	m_Alive = other.m_Alive;
	m_Solid = other.m_Solid;
	m_bb = other.m_bb;
	if (other.m_ID)
	{
		size_t length = strlen(other.m_ID);
		m_ID = new char[length + 1];
		if (strcpy_s(m_ID, length + 1, other.m_ID) != 0)
		{
			std::cerr << "[GameObject][ERROR] Failed to copy ID\n";
		}
	}
	else
		m_ID = nullptr;
}

GameObject::GameObject(GameObject&& other) noexcept
	: m_Position(other.m_Position), m_Size(other.m_Size), m_Velocity(other.m_Velocity),
	m_Rotation(other.m_Rotation), m_Texture(std::move(other.m_Texture)), m_Color(other.m_Color),
	m_Alive(other.m_Alive), m_Solid(other.m_Solid), m_bb(other.m_bb), m_ID(other.m_ID)
{
	other.m_ID = nullptr;
}

GameObject& GameObject::operator=(const GameObject& other)
{
	if (this != &other)
	{
		// First, delete the existing m_ID to prevent memory leak
		delete[] m_ID;
		m_ID = nullptr;

		m_Position = other.m_Position;
		m_Size = other.m_Size;
		m_Velocity = other.m_Velocity;
		m_Rotation = other.m_Rotation;
		m_Texture = other.m_Texture;
		m_Color = other.m_Color;
		m_Alive = other.m_Alive;
		m_Solid = other.m_Solid;
		m_bb = other.m_bb;

		if (other.m_ID)
		{
			size_t length = strlen(other.m_ID);
			m_ID = new char[length + 1];
			if (strcpy_s(m_ID, length + 1, other.m_ID) != 0)
			{
				std::cerr << "[GameObject][ERROR] Failed to copy ID\n";
			}
		}
	}
	return *this;
}

GameObject& GameObject::operator=(GameObject&& other) noexcept
{
	if (this != &other)
	{
		delete[] m_ID;
		m_ID = other.m_ID;
		other.m_ID = nullptr;
		m_Position = other.m_Position;
		m_Size = other.m_Size;
		m_Velocity = other.m_Velocity;
		m_Rotation = other.m_Rotation;
		m_Texture = other.m_Texture;
		m_Color = other.m_Color;
		m_Alive = other.m_Alive;
		m_Solid = other.m_Solid;
		m_bb = other.m_bb;
	}
	return *this;
}


GameObject::~GameObject()
{
	// std::println("[GameObject] Cleaning and deleting resources...");
	delete[] m_ID;
	// std::println("[GameObject] Cleaned up.");
}

void GameObject::Render(SpriteRenderer* renderer) const
{
	renderer->DrawSprite(m_Texture, m_Position, m_Rotation, m_Size, m_Color, m_Solid);
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

void GameObject::UpdateBoundingBox()
{
	m_bb.min = m_Position;
	m_bb.max = m_Position + m_Size;
}