#pragma once
#include "SpriteRenderer.h"
#include "object/GameObject.h"

class CollisionDebugSpriteRenderer : public SpriteRenderer
{
public:
	CollisionDebugSpriteRenderer(ShaderPtr shader) : SpriteRenderer(shader), m_IsBall(false) {}

	void DrawSprite(Texture2DPtr texture, glm::vec2 position, float rotate, glm::vec2 size, glm::vec3 color, bool isBrick = false) override
	{
		SpriteRenderer::GetShader()->SetBool("is_ball", m_IsBall);
		SpriteRenderer::GetShader()->SetBool("has_texture", (bool)texture);
		SpriteRenderer::DrawSprite(texture, position, rotate, size, color, isBrick);
		SpriteRenderer::GetShader()->SetBool("is_ball", false);
	}
	void SetBallType(ObjectType type)
	{
		m_IsBall = type == ObjectType::Ball ? true : false;
	}
private:
	bool m_IsBall{ false };
};