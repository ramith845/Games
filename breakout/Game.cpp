#include "pch.h"
#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Random.h"

#define GET_SHADER(x) ResourceManager::GetShader(x)
#define GET_TEXTURE(x) ResourceManager::GetTexture(x)

#ifdef _DEBUG
constexpr const char* SPRITE_IMG = "../resources/awesomeface.png";
#else
constexpr const char* SPRITE_IMG{ "resources/awesomeface.png" };
#endif // _DEBUG


glm::vec2 dir{};

SpriteRenderer* renderer;

Game::Game(unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height)
	, m_State(GameState::MENU), m_Keys()
{

}

Game::Game(int width, int height)
	: Game(static_cast<unsigned int>(width), static_cast<unsigned int>(height))
{
	if (width < 0 || height < 0) {
		throw std::invalid_argument("Width and height must be non-negative.");
	}
}

Game::~Game()
{

}

int Game::Init()
{
	ResourceManager::LoadShader("shaders/3.3.shader.vert", "shaders/3.3.shader.frag", "sprite_shader");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);
	GET_SHADER("sprite_shader")->use();
	GET_SHADER("sprite_shader")->SetInt("image", 0);
	GET_SHADER("sprite_shader")->SetMatrix4("projection", projection);

	ResourceManager::LoadTexture(SPRITE_IMG, "sprite");

	renderer = new SpriteRenderer(GET_SHADER("sprite_shader"));

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 100);

	// Generate a number
	dir = glm::normalize(glm::vec2{ Random::get<float>(0, 1), Random::get<float>(0, 1) });
	m_Velocity = std::max(m_Width, m_Height) / 2.0f;

	return 1;
}

void Game::ProcessInput(float dt)
{
	dt;
}

void Game::Update(float dt)
{
	m_Position += dt * (m_Width / 2.f) * dir;

	bool xOut = m_Position.x + m_Size.x >= m_Width || m_Position.x <= 0;
	bool yOut = m_Position.y + m_Size.y >= m_Height || m_Position.y <= 0;
	if ((xOut || yOut) && xOut)
		dir.x *= -1;
	else if ((xOut || yOut) && yOut)
		dir.y *= -1;

	std::cout << m_Position.x << ", " << m_Position.y << "\n";
}

void Game::Run()
{
	renderer->Draw(GET_TEXTURE("sprite"),
		m_Position,
		0.f,
		m_Size,
		glm::vec3(1.0));
}

