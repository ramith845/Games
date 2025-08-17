#pragma once
#include "Shader.h"
#include "Texture.h"
#include <memory>

class SpriteRenderer
{
public:
	SpriteRenderer(ShaderPtr shader);
	~SpriteRenderer();

	void Init();
	ShaderPtr GetShader() const;
	virtual void DrawSprite(Texture2DPtr texture, glm::vec2 position, float rotate, glm::vec2 size, glm::vec3 color, bool isBrick = false);
private:
	unsigned int m_VAO{}, m_VBO{};
	ShaderPtr m_Shader;
};

