#pragma once
#include "renderer/Texture.h"
#include "renderer/SpriteRenderer.h"

struct BoundinBox
{
	glm::vec2 min{};
	glm::vec2 max{};
};

class GameObject
{
public:
	GameObject(glm::vec2 position, glm::vec2 size, Texture2D* texture, bool solid,
		glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2{ 0.0f },
		float rotation = 0.0f);
	GameObject() = delete;
	~GameObject();

	static void ResetSidCounter();
	const char* GetID() const;
	virtual void Render(SpriteRenderer* renderer);
	virtual glm::vec2 GetCenter();

public:
	glm::vec2 m_Position{ 0.f }, m_Size{ 10.f }, m_Velocity{ 0.f };
	float m_Rotation{ 0.f };
	Texture2D* m_Texture{};
	glm::vec3 m_Color{ 1.0 };
	bool m_Alive{ true };
	const bool m_Solid{ false };
	BoundinBox m_bb{};
private:
	char* m_ID{};
	inline static unsigned int s_IdCounter;
};

