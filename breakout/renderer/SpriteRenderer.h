#pragma once
#include "Shader.h"
#include "Texture.h"

class SpriteRenderer
{
public:
	SpriteRenderer(Shader* shader);
	~SpriteRenderer();

	void Init();

	void DrawSprite(Texture2D* texture, glm::vec2 position, float rotate, glm::vec2 size, glm::vec3 color, bool isBrick = false);
private:
	unsigned int m_VAO{}, m_VBO{};
	Shader* m_Shader;
};

