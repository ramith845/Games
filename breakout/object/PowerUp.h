#pragma once
#include "GameObject.h"
#define PWR_TYPE(type) PowerUp::PowerTypeString(PowerUp::##type)
#define PWR_TYPE_INT(type) PowerUp::PowerTypeString(static_cast<PowerUp::Type>(type))

constexpr const glm::vec2 SIZE(60.0f, 20.0f);
constexpr const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp :
	public GameObject
{
public:
	enum Type
	{
		Chaos,
		DecreasePaddleSize,
		IncreaseBallVelocity,
		Confuse,
		DecreaseBallVelocity,
		IncreasePaddleSize,
		StickyPaddle,
		PassThrough,
		BonusLife,
		//FireBall,
		End
	};
public:
	PowerUp(const char* type, float duration,
		glm::vec2 position, Texture2DPtr texture, glm::vec3 color,
		glm::vec2 size = SIZE, glm::vec2 velocity = VELOCITY,
		float rotation = 0.0f)
		:GameObject(position, size, texture, ObjectType::PowerUp, color, velocity, rotation)
		, m_Type(type), m_Duration(duration), m_Activated()
	{
	}
	PowerUp() = delete;
	static std::string_view PowerTypeString(Type type);
	const char* m_Type;
	float       m_Duration;
	bool        m_Activated{ false };
	inline static int s_Weights[Type::End];
	inline static int s_TotalWeight{ 0 };
	inline static glm::vec2 s_Size{ SIZE };
	inline static glm::vec2 s_Velocity{ VELOCITY };
};

