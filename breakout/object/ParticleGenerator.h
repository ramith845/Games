#include "Particle.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "object/GameObject.h"
#include "object/BallObject.h"

class ParticleGenerator
{
public:
	ParticleGenerator(Shader* shader, Texture2D* texture, unsigned int nrParticles);
	~ParticleGenerator();

	void Init();
	void Draw();
	void Update(float dt, BallObject& obj, unsigned int newParticles, 
		glm::vec2 offset = glm::vec2(0.0f, 0.0f));


private:
	void RespawnParticle(Particle& particle, BallObject& obj, glm::vec2 offset);
	unsigned int FirstUnusedParticle();

public:
	Shader* m_Shader{};
	Texture2D* m_Texture{};
	glm::vec3 m_Color{ 1.0 };
private:
	unsigned int m_VAO{};
	unsigned int m_NrParticles{ 500 };
	unsigned int m_LastUsedParticle{ 0 };
	std::vector<Particle> m_Particles;
	char* m_ID{};
	inline static unsigned int s_IdCounter;
};

