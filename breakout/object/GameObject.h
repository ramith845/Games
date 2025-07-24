#pragma once
#include "renderer/Texture.h"
#include "renderer/SpriteRenderer.h"

class GameObject
{
public:
	GameObject(glm::vec2 position, glm::vec2 size, Texture2D* texture,
		glm::vec3 color, float velocity = 0.0f, float rotation = 0.0f
	);

	GameObject(float posX, float posY, float sizeX, float sizeY,
		float colorR, float colorG, Texture2D* texture, float colorB,
		float velocity, float rotation
	);
	GameObject() = delete;
	~GameObject();

	static void ResetSidCounter();
	const char* GetID() const;
	virtual void Render(SpriteRenderer* renderer);

public:
	glm::vec2 m_Position{ 0.f }, m_Size{ 10.f };
	float m_Velocity{ 0.f };
	float m_Rotation{ 0.f };
	Texture2D* m_Texture{};
	glm::vec3 m_Color{ 1.0 };
	bool m_Alive{};
	bool m_Solid{};

private:
	char* m_ID{};
	inline static unsigned int s_IdCounter;
};

