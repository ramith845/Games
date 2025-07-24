#include "pch.h"
#include "Game.h"
#include "manager/ResourceManager.h"
#include "renderer/SpriteRenderer.h"
#include "utils/Random.h"
#include "level/GameLevel.h"


constexpr const char* BACKGROUND_IMG = "resources/background.jpg";
constexpr const char* AWESOMEFACE_IMG = "resources/awesomeface.png";
constexpr const char* BLOCK_IMG = "resources/block.png";
constexpr const char* BLOCK_SOLID_IMG = "resources/block_solid.png";
constexpr const char* PADDLE_IMG = "resources/paddle.png";


// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);

//glm::vec2 dir{};

SpriteRenderer* renderer;
GameObject* Player;

Game::Game(unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height), m_Level(0)
	, m_State(GameState::ACTIVE), m_Keys(), m_DeltaTime(0.f)
{
	m_Window = std::make_unique<Window>(width, height, "Game");
	m_Window->SetCBEventfn(std::bind(&Game::OnEvent, this, std::placeholders::_1));
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
	std::println("[Game] Cleaning up and deleting resources...");

	std::println("	- deleting renderer(sprite)");
	delete renderer;

	std::println("	- deleting Player(Object)");
	delete Player;

	ResourceManager::Cleanup();

	glfwTerminate();
	std::println("[Game] Cleanup complete.");
}


int Game::Init()
{
	ResourceManager::LoadShader("shaders/3.3.shader.vert", "shaders/3.3.shader.frag", "sprite_shader");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);
	GET_SHADER("sprite_shader")->use();
	GET_SHADER("sprite_shader")->SetInt("image", 0);
	GET_SHADER("sprite_shader")->SetMatrix4("projection", projection);

	ResourceManager::LoadTexture(BACKGROUND_IMG, "background");
	ResourceManager::LoadTexture(AWESOMEFACE_IMG, "face");
	ResourceManager::LoadTexture(BLOCK_IMG, "block");
	ResourceManager::LoadTexture(BLOCK_SOLID_IMG, "block_solid");
	ResourceManager::LoadTexture(PADDLE_IMG, "paddle");

	renderer = new SpriteRenderer(GET_SHADER("sprite_shader"));


	std::vector<const char*> levelPaths{ "levels/one.lvl", "levels/two.lvl", "levels/three.lvl", "levels/four.lvl" };
	for (const char* path : levelPaths) {
		auto level = std::make_unique<GameLevel>();
		level->LoadLevel(path, m_Width, m_Height / 2);
		m_Levels.push_back(std::move(level));
	}
	m_Level = 0;

	glm::vec2 playerInitialPos = glm::vec2((m_Width - PLAYER_SIZE.x) / 2.0, m_Height - PLAYER_SIZE.y);
	Player = new GameObject{
		playerInitialPos,
		PLAYER_SIZE,
		GET_TEXTURE("paddle"),
		glm::vec3(1.0),
		PLAYER_VELOCITY };

	return 1;
}


void Game::OnEvent(Event& e)
{
	using type = decltype(Game::OnKeyPress);
	EventDispatcher handler{ e };
	handler.Dispatch<KeyPressEvent>(std::bind(&Game::OnKeyPress, this, std::placeholders::_1));
	handler.Dispatch<KeyReleaseEvent>(std::bind(&Game::OnKeyRelease, this, std::placeholders::_1));
}

bool Game::OnKeyPress(KeyPressEvent& ke)
{
	m_Keys[static_cast<size_t>(ke.GetKeyCode())] = true;
	std::println("Key press: {}", (char)ke.GetKeyCode());
	return true;
}

bool Game::OnKeyRelease(KeyReleaseEvent& kr)
{
	m_Keys[static_cast<size_t>(kr.GetKeyCode())] = false;
	std::println("Key release: {}", (char)kr.GetKeyCode());
	return true;
}

void Game::ProcessInput()
{
	if (m_Keys[GLFW_KEY_D] && Player->m_Position.x + Player->m_Size.x <= m_Width)
		Player->m_Position.x += m_DeltaTime * PLAYER_VELOCITY;
	else if (m_Keys[GLFW_KEY_A] && Player->m_Position.x >= 0.0f)
		Player->m_Position.x -= m_DeltaTime * PLAYER_VELOCITY;
}

void Game::Update()
{
}

static float lastFrame = 0.0f;
void Game::Run()
{
	while (!m_Window->IsWindowClosed())
	{
		float currentFrame = static_cast<float>(glfwGetTime()); // seconds since program start
		m_DeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//std::cout << deltaTime << "\n";

		glClearColor(1.0, 1.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		ProcessInput();
		//Update(deltaTime);
		if (m_State == GameState::ACTIVE)
		{
			// BACKGROUND IMAGE/OBJ FIRST
			renderer->Draw(
				GET_TEXTURE("background"),
				glm::vec3(0.0f),
				0.0f,
				glm::vec2(m_Width, m_Height),
				glm::vec3(1.0));

			// PLAYER/PADDLE
			Player->Render(renderer);

			// LEVELS
			m_Levels[m_Level]->Draw(renderer);
		}

		m_Window->Update();
	}
}
