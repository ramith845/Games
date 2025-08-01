#include "pch.h"
#include "ParticleGenerator.h"
#include "Particle.h"
#include "object/GameObject.h"
#include "utils/Random.h"

ParticleGenerator::ParticleGenerator(Shader* shader, Texture2D* texture, unsigned int nrParticles)
	:m_Shader(shader), m_Texture(texture), m_NrParticles(nrParticles), m_LastUsedParticle(0U)
{
	this->Init();
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::Init()
{
	unsigned int VBO;
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

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	for (unsigned int i = 0; i < m_NrParticles; i++)
	{
		m_Particles.push_back(Particle());
	}
}

void ParticleGenerator::Draw()
{

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	m_Shader->use();
	glm::mat4 model(1.0);
	m_Shader->SetMatrix4("model", model);
	for (auto& particle : m_Particles)
	{
		if (particle.Life >= 0)
		{
			m_Shader->SetVec2("offset", particle.Position);
			m_Shader->SetVec2("size", particle.Size);
			m_Shader->SetVec4("color", particle.Color);

			glActiveTexture(GL_TEXTURE0);
			m_Texture->Bind();

			glBindVertexArray(m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::RespawnParticle(Particle& particle, BallObject& obj, glm::vec2 offset)
{
	float angle = Random::get(0.f, 2.f * glm::pi<float>());
	float edgeOffset = Random::get(0.f, obj.m_Size.x / 2.f);
	glm::vec2 randDir = glm::vec2(cos(angle), sin(angle));
	particle.Position = (obj.GetCenter() - particle.Size / 2.0f) + randDir * edgeOffset + offset;

	particle.Size = glm::vec2(Random::get(5.f, 10.f));
	particle.Velocity = 0.1f * obj.m_Velocity;
	particle.Life = 1.0f;

	float randColorR = Random::get(0.f, 1.f);
	float randColorG = Random::get(0.f, 1.f);
	float randColorB = Random::get(0.f, 1.f);
	particle.Color = glm::vec4(randColorR, randColorG, randColorB, 1.0);
}


unsigned int ParticleGenerator::FirstUnusedParticle()
{
	unsigned int i = m_LastUsedParticle;
	for (unsigned int cnt{ 0 }; cnt < m_NrParticles; cnt++)
	{
		if (m_Particles[i].Life <= 0.f)
		{
			m_LastUsedParticle = i;
			return i;
		}
		i = (i + 1) % m_NrParticles;
	}
	m_LastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::Update(float dt, BallObject& obj, unsigned int newParticles,
	glm::vec2 offset)
{
	//unsigned int nr_new_particles = 2;
	for (unsigned int i = 0; i < newParticles; i++)
	{
		RespawnParticle(m_Particles[FirstUnusedParticle()], obj, offset);
	}

	for (unsigned int i = 0; i < m_NrParticles; i++)
	{
		Particle& p = m_Particles[i];
		p.Life -= dt; // Decrease life
		if (p.Life > 0.f)
		{
			p.Position -= dt * p.Velocity * 1.5f; // Update position
			p.Color.a -= dt * 5.5f; // Update alpha based on life
		}
	}
}
