#pragma once
#include "renderer/Texture.h"
#include "renderer/SpriteRenderer.h"
#include <memory>

struct BoundinBox
{
	glm::vec2 min{};
	glm::vec2 max{};
};

class GameObject
{
public:
	GameObject(glm::vec2 position, glm::vec2 size, Texture2DPtr texture, bool solid,
		glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2{ 0.0f },
		float rotation = 0.0f);
	GameObject() = delete;
	// Rule of three
	GameObject(const GameObject& other);
	GameObject(GameObject&& other) noexcept;
	GameObject& operator=(const GameObject& other);
	GameObject& operator=(GameObject&& other) noexcept;
	~GameObject();


	static void ResetSidCounter();
	const char* GetID() const;
	virtual void Render(SpriteRenderer* renderer) const;
	virtual glm::vec2 GetCenter();
	virtual void UpdateBoundingBox();

public:
	glm::vec2 m_Position{ 0.f }, m_Size{ 10.f }, m_Velocity{ 0.f };
	float m_Rotation{ 0.f };
	Texture2DPtr m_Texture{};
	glm::vec3 m_Color{ 1.0 };
	bool m_Alive{ true };
	bool m_Solid{ false };
	BoundinBox m_bb{};
#ifdef BRICK_DEBUG
	bool m_Hit{ false };
#endif // BRICK_DEBUG

private:
	char* m_ID{};
	inline static unsigned int s_IdCounter;
};

