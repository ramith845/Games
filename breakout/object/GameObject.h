#pragma once
#include "renderer/Texture.h"
#include "renderer/SpriteRenderer.h"
#include <memory>

struct BoundinBox
{
	glm::vec2 min{};
	glm::vec2 max{};
};

enum class ObjectType
{
	None = 0,
	Brick,
	SolidBrick,
	Player,
	Ball,
	Particle,
	PowerUp,
	Effect,
	DebugBox
};

class GameObject
{
public:
	GameObject(glm::vec2 position, glm::vec2 size, Texture2DPtr texture, ObjectType type = ObjectType::None,
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
	const ObjectType GetType() const;
	glm::vec2 GetPos() const;
	virtual glm::vec2 GetCenter() const;

	virtual void SetPos(glm::vec2 position);
	virtual void SetPosX(float x);
	virtual void SetPosY(float y);
	virtual void Render(SpriteRenderer* renderer) const;
private:
	virtual void UpdateBoundingBox();

public:
	glm::vec2 m_Size{ 10.f }, m_Velocity{ 0.f };
	float m_Rotation{ 0.f };
	Texture2DPtr m_Texture{};
	glm::vec3 m_Color{ 1.0 };
	bool m_Alive{ true };
	bool m_Solid{ false };
	BoundinBox m_bb{};

private:
	glm::vec2 m_Position{ 0.f };
	char* m_ID{};
	inline static unsigned int s_IdCounter;
	ObjectType m_Type{ ObjectType::None };
};

