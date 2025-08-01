#include "pch.h"
#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader* shader)
	: m_Shader(shader), m_VAO(0), m_VBO(0)
{
	Init();
}

SpriteRenderer::~SpriteRenderer()
{
	std::println("[SpriteRenderer] Cleaning and deleting resources...");
	std::println("	- deleting vertex buffer: {}", m_VBO);
	glDeleteBuffers(1, &m_VBO);
	std::println("	- deleting vertex array: {}", m_VAO);
	glDeleteVertexArrays(1, &m_VAO);
	std::println("[SpriteRenderer] Cleaned up.");
}

void SpriteRenderer::Init()
{
	// configure VAO/VBO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//unsigned int VBO;
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D* texture, glm::vec2 position, float rotate, glm::vec2 size, glm::vec3 color, bool isBrick)
{
	m_Shader->use();

	glm::mat4 model(1.0);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5 * position.x, 0.5 * position.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(-0.5 * position.x, -0.5 * position.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0));

	m_Shader->SetMatrix4("model", model);
	m_Shader->SetVec3("spriteColor", color);
	m_Shader->SetBool("is_brick", isBrick);
	if (isBrick)
	{
		m_Shader->SetVec2("brick_size", size);
	}

	glActiveTexture(GL_TEXTURE0);
	texture->Bind();

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}