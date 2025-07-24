#include "pch.h"
#include "GameObject.h"
#include "utils/Random.h"

unsigned int s_IdCounter{ 0 };

GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2D* texture,
	glm::vec3 color, float velocity, float rotation
)
	: m_Position(position), m_Size(size), m_Velocity(velocity)
	, m_Rotation(rotation), m_Texture(texture), m_Color(color)
{
	/*std::stringstream ss;
	ss << std::fixed << std::setprecision(3)
		<< position.x << position.y << "_"
		<< color.x << color.g << color.b << "_"
		<< Random::get(1.f, 100.f) << "_" << texture->GetID() << s_IdCounter++;*/

	std::string tmp = std::format("{}{}_{}{}{}_{}_{}{}",
		position.x, position.y,
		color.x, color.g, color.b,
		Random::get(1.f, 100.f),
		texture->GetID(), s_IdCounter++);
	//m_ID = tmp.c_str();

	m_ID = new char[tmp.size() + 1];
	errno_t result = strcpy_s(const_cast<char*>(m_ID), tmp.size() + 1, tmp.c_str());
	if (result != 0) {
		std::cout << "[GameObject][ERROR] Failed to copy ID\n";
	}
}

GameObject::GameObject(float posX, float posY, float sizeX, float sizeY,
	float colorR, float colorG, Texture2D* texture, float colorB,
	float velocity, float rotation
)
	: GameObject(glm::vec2(posX, posY), glm::vec2(sizeX, sizeY),
		texture, glm::vec3(colorR, colorG, colorB),
		velocity, rotation
	) {
}


GameObject::~GameObject()
{
	std::println("[GameObject] Cleaning and deleting resources...");
	delete[] m_ID;
	std::println("[GameObject] Cleaned up.");
}

void GameObject::Render(SpriteRenderer* renderer)
{
	renderer->Draw(m_Texture, m_Position, m_Rotation, m_Size, m_Color);
}

const char* GameObject::GetID() const
{
	return m_ID;
}

void GameObject::ResetSidCounter()
{
	s_IdCounter = 0;
}