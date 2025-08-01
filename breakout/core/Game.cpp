#include "pch.h"
#include "Game.h"
#include "level/GameLevel.h"
#include "object/PowerUp.h"
#include "object/GameObject.h"
#include "object/BallObject.h"
#include "object/ParticleGenerator.h"
#include "manager/ResourceManager.h"
#include "renderer/SpriteRenderer.h"
#include "renderer/PostProcessor.h"
#include "utils/Random.h"
#include "utils/Utility.h"


constexpr const char* BACKGROUND_IMG = "resources/background1_edit.jpg";
constexpr const char* AWESOMEFACE_IMG = "resources/awesomeface.png";
constexpr const char* BLOCK_IMG = "resources/block.png";
constexpr const char* BLOCK_SOLID_IMG = "resources/block_solid.png";
constexpr const char* PADDLE_IMG = "resources/paddle.png";
constexpr const char* PARTICLE_IMG = "resources/particle.png";

constexpr const char* POWERUP_TEXTURE_PATHS[] = {
	"resources/powerup_chaos.png",          // Chaos
	"resources/powerup_decrease.png",       // DecreasePaddleSize
	"resources/powerup_speed.png",          // IncreaseBallVelocity
	"resources/powerup_confuse.png",        // Confuse
	"resources/powerup_slow.png",           // DecreaseBallVelocity
	"resources/powerup_increase.png",       // IncreasePaddleSize
	"resources/powerup_sticky.png",         // StickyPaddle
	"resources/powerup_passthrough.png",    // PassThrough
	"resources/powerup_life.png" // BonusLife (make sure this file exists)
};
constexpr const char* POWERUP_TEXTURE_NAMES[] = {
	"powerup_chaos",            // Chaos
	"powerup_decrease",        // DecreasePaddleSize
	"powerup_speed",           // IncreaseBallVelocity
	"powerup_confuse",         // Confuse
	"powerup_slow",            // DecreaseBallVelocity
	"powerup_increase",        // IncreasePaddleSize
	"powerup_sticky",          // StickyPaddle
	"powerup_passthrough",     // PassThrough
	"powerup_bonus_life"       // BonusLife (assumed name)
};


// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Player inc/dec size power factor
const glm::vec2 PLAYER_SIZE_PWRUP_FACTOR(50.0f, 0.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(250.0f);
// Ball initial velocity
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 12.5f;

static float lastFrame = 0.0f;
static float currentFrame;
static float fpsLastFrame = 0.0f;
static int counter{ 0 };
static float ShakeTime = 0.0f;

SpriteRenderer* renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;
PostProcessor* Effects;

Game::Game(unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height), m_Level(0)
	, m_State(GameState::ACTIVE), m_Keys(), m_DeltaTime(0.f)
{
	m_Window = std::make_unique<Window>(width, height, "Game");
	if (!m_Window->IsSuccess())
	{
		throw std::runtime_error("Failed to create window!");
	}
	m_Window->SetCBEventfn(std::bind(&Game::OnEvent, this, std::placeholders::_1));
}

Game::Game(int width, int height)
	: Game(static_cast<unsigned int>(width), static_cast<unsigned int>(height))
{
	if (width < 0 || height < 0) {
		throw std::invalid_argument("Width and height must be non-negative.");
	}

	PowerUp::s_Weights[PowerUp::Chaos] = 50;
	PowerUp::s_Weights[PowerUp::DecreasePaddleSize] = 50;
	PowerUp::s_Weights[PowerUp::IncreaseBallVelocity] = 50;
	PowerUp::s_Weights[PowerUp::Confuse] = 40;
	PowerUp::s_Weights[PowerUp::DecreaseBallVelocity] = 15;
	PowerUp::s_Weights[PowerUp::IncreasePaddleSize] = 15;
	PowerUp::s_Weights[PowerUp::StickyPaddle] = 15;
	PowerUp::s_Weights[PowerUp::PassThrough] = 15;
	PowerUp::s_Weights[PowerUp::BonusLife] = 10;

	for (int i = 0; i < PowerUp::End; i++)
	{
		PowerUp::s_TotalWeight += PowerUp::s_Weights[i];
	}
}

Game::~Game()
{
	std::println("[Game] Cleaning up and deleting resources...");

	std::println("	- deleting renderer(sprite)");
	delete renderer;

	std::println("	- deleting Player(Object)");
	delete Player;

	std::println("	- deleting Ball(Object)");
	delete Ball;

	ResourceManager::Cleanup();

	std::println("[Game] Cleanup complete.");
}

int Game::Init()
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::LoadShader("shaders/3.3.shader.vert", "shaders/3.3.shader.frag", "sprite_shader");
	GET_SHADER("sprite_shader")->use();
	GET_SHADER("sprite_shader")->SetInt("image", 0);
	GET_SHADER("sprite_shader")->SetMatrix4("projection", projection);

	ResourceManager::LoadShader("shaders/3.3.particle.vert", "shaders/3.3.particle.frag", "particle_shader");

	GET_SHADER("particle_shader")->use();
	GET_SHADER("particle_shader")->SetInt("image", 0);
	GET_SHADER("particle_shader")->SetMatrix4("projection", projection);


	// TEXTURE LOADING
	ResourceManager::LoadTexture(BACKGROUND_IMG, "background");
	ResourceManager::LoadTexture(AWESOMEFACE_IMG, "face");
	ResourceManager::LoadTexture(BLOCK_IMG, "block");
	ResourceManager::LoadTexture(BLOCK_SOLID_IMG, "block_solid");
	ResourceManager::LoadTexture(PADDLE_IMG, "paddle");
	ResourceManager::LoadTexture(PARTICLE_IMG, "particle");
	// POWERUP TEXTURES
	for (int i = 0; i < PowerUp::Type::End; ++i) {
		ResourceManager::LoadTexture(POWERUP_TEXTURE_PATHS[i], POWERUP_TEXTURE_NAMES[i]);
	}



	renderer = new SpriteRenderer(GET_SHADER("sprite_shader"));


	std::vector<const char*> levelPaths{ "levels/one.lvl", "levels/two.lvl", "levels/three.lvl", "levels/four.lvl" };
	for (const char* path : levelPaths) {
		auto level = std::make_unique<GameLevel>();
		level->LoadLevel(path, m_Width, m_Height / 2);
		m_Levels.push_back(std::move(level));
	}
	m_Level = 0;

	glm::vec2 playerInitialPos = glm::vec2(383.7f, m_Height - PLAYER_SIZE.y);
	Player = new GameObject{
		playerInitialPos,
		PLAYER_SIZE,
		GET_TEXTURE("paddle"),
		true,
		glm::vec3(1.0),
		glm::vec2(PLAYER_VELOCITY) };

	glm::vec2 ballInitialPos{ playerInitialPos.x + PLAYER_SIZE.x / 2.0f - BALL_RADIUS,  playerInitialPos.y - 2.f * BALL_RADIUS };
	Ball = new BallObject(ballInitialPos, BALL_RADIUS, GET_TEXTURE("face"), INITIAL_BALL_VELOCITY);
	Ball->m_Stuck = true;

	Particles = new ParticleGenerator(GET_SHADER("particle_shader"), GET_TEXTURE("particle"), 500);

	Effects = new PostProcessor(m_Width, m_Height);


	return 1;
}

void Game::OnEvent(Event& e)
{
	using type = decltype(Game::OnKeyPress);
	EventDispatcher handler{ e };
	handler.Dispatch<KeyPressEvent>(std::bind(&Game::OnKeyPress, this, std::placeholders::_1));
	handler.Dispatch<KeyReleaseEvent>(std::bind(&Game::OnKeyRelease, this, std::placeholders::_1));
	handler.Dispatch<WindowCloseEvent>(std::bind(&Game::OnCloseGame, this, std::placeholders::_1));
	handler.Dispatch<WindowResizeEvent>(std::bind(&Game::OnResizeGame, this, std::placeholders::_1));
}

bool Game::OnCloseGame(WindowCloseEvent& e)
{
	e;
	m_State = GameState::TERMINATE;
	return true;
}

bool Game::OnResizeGame(WindowResizeEvent& e)
{
	m_Width = e.GetWidth();
	m_Height = e.GetHeight();
	m_Window->Viewport(static_cast<int>(m_Width), static_cast<int>(m_Height));
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);
	GET_SHADER("sprite_shader")->use()->SetMatrix4("projection", projection);
	return true;
}

bool Game::OnKeyRelease(KeyReleaseEvent& e)
{
	m_Keys[static_cast<size_t>(e.GetKeyCode())] = false;
	std::println("Key release: {}", (char)e.GetKeyCode());
	return true;
}

bool Game::OnKeyPress(KeyPressEvent& e)
{
	m_Keys[static_cast<size_t>(e.GetKeyCode())] = true;
	std::println("Key press: {}", (char)e.GetKeyCode());
	return true;
}

glm::vec2 clamp(glm::vec2 val, glm::vec2 min, glm::vec2 max)
{
	return
	{
		std::max(min.x, std::min(max.x, val.x)),
		std::max(min.y, std::min(max.y, val.y))
	};
}

Direction CollisionDirection(glm::vec2 vec)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	float max{ 0.0f };
	int match{ -1 };
	for (int i = 0; i < Direction::END; i++)
	{
		float dotprod = glm::dot(glm::normalize(vec), compass[i]);
		if (dotprod > max)
		{
			max = dotprod;
			match = i;
		}
	}
	return static_cast<Direction>(match);
}

Collision_t Game::CheckCollision(BallObject& one, GameObject& two)
{
	// square based collision check AABB - naive approach for any shape
	//return one.m_bb.min <= two.m_bb.max && two.m_bb.min <= one.m_bb.max; // overloaded op<=
	glm::vec2 C{ one.GetCenter() };
	glm::vec2 aabbCenter{ two.GetCenter() };

	glm::vec2 vec = C - aabbCenter; // center to center vec

	// origin about object two
	glm::vec2 aabb_half_extents(two.m_Size.x / 2.0f, two.m_Size.y / 2.0f);
	// clamping/intersection center-center vec to aabb
	glm::vec2 clampedVec{ clamp(vec, -aabb_half_extents, aabb_half_extents) };
	// clamped position
	glm::vec2 nearestPos = aabbCenter + clampedVec;
	// object center-nearest AABB vector
	glm::vec2 nearestVec = nearestPos - C;
	// vec smaller than object radius?
	bool collison = glm::length(nearestVec) <= one.m_Radius;
	if (collison)
	{
		return std::make_tuple(true, CollisionDirection(nearestVec), nearestVec);
	}
	else
	{
		return std::make_tuple(false, UP, glm::vec2(0.0f));
	}
}

void Game::DoCollisions(BallObject& ball)
{
	// Check collision with all boxes in the current level
	for (GameObject* box : m_Levels[m_Level]->GetEnt())
	{
		if (!box->m_Alive)
			continue;

		Collision_t collision = CheckCollision(ball, *box);
		if (std::get<0>(collision))
		{
			if (!box->m_Solid)
			{
				box->m_Alive = false;
			}
			else
			{
				ShakeTime = 0.05f;
				Effects->m_Shake = true;
			}
			Direction dir = std::get<1>(collision);
			glm::vec2 diff = std::get<2>(collision);
			if (dir == LEFT || dir == RIGHT)
			{
				float penetration = ball.m_Radius - std::abs(diff.x);
				ball.m_Velocity.x *= -1;
				if (dir == LEFT)
					ball.m_Position.x -= penetration;
				else
					ball.m_Position.x += penetration;
			}
			else
			{
				float penetration = ball.m_Radius - std::abs(diff.y);
				ball.m_Velocity.y *= -1;
				if (dir == UP)
					ball.m_Position.y -= penetration;
				else
					ball.m_Position.y += penetration;
			}
			break;
		}
	}

	// Check collision with player paddle
	Collision_t playa = CheckCollision(ball, *Player);
	if (!Ball->m_Stuck && std::get<0>(playa))
	{
		if (std::get<1>(playa) == UP)
		{
			glm::vec2 CPlaya = Player->GetCenter();
			glm::vec2 CBall = ball.GetCenter();
			float distance = CBall.x - CPlaya.x;
			float per = distance / (Player->m_Size.x / 2.0f);
			float strength = 2.0f;
			glm::vec2 oldVelocty = ball.m_Velocity;
			ball.m_Velocity.x = strength * per * INITIAL_BALL_VELOCITY.x;
			ball.m_Velocity.y = -1.f * abs(ball.m_Velocity.y);
			ball.m_Velocity = glm::normalize(ball.m_Velocity) * glm::length(oldVelocty);
		}
	}
}

float GetPowerupDuration(std::string_view type)
{
	if (type == PWR_TYPE(Chaos))
		return 15.0f;
	else if (type == PWR_TYPE(DecreasePaddleSize))
		return 0.0f;
	else if (type == PWR_TYPE(IncreaseBallVelocity))
		return 10.0f;
	else if (type == PWR_TYPE(Confuse))
		return 15.0f;
	else if (type == PWR_TYPE(DecreaseBallVelocity))
		return 20.0f;
	else if (type == PWR_TYPE(IncreasePaddleSize))
		return 0.0f;
	else if (type == PWR_TYPE(StickyPaddle))
		return 20.0f;
	else if (type == PWR_TYPE(PassThrough))
		return 10.0f;
	else if (type == PWR_TYPE(BonusLife))
		return 0.0f;
	std::println("Unknown powerup encountered");
	return 5.0f;
}

void Game::SpawnPowerups(GameObject& obj)
{
	obj;
	int chance = /*Random::get(0, PowerUp::s_TotalWeight)*/1;
	int cumulativeWeight = 0;
	const glm::vec3 POWERUP_COLORS[] = {
		glm::vec3(0.9f, 0.25f, 0.25f),  // Chaos
		glm::vec3(1.0f, 0.6f, 0.4f),    // DecreasePaddleSize (reusing increase for balance)
		glm::vec3(0.5f, 0.5f, 1.0f),    // IncreaseBallVelocity (speed)
		glm::vec3(1.0f, 0.3f, 0.3f),    // Confuse
		glm::vec3(0.3f, 0.3f, 1.0f),    // DecreaseBallVelocity (guessed cooler dim blue)
		glm::vec3(1.0f, 0.6f, 0.4f),    // IncreasePaddleSize
		glm::vec3(1.0f, 0.5f, 1.0f),    // StickyPaddle
		glm::vec3(0.5f, 1.0f, 0.5f),    // PassThrough
		glm::vec3(0.6f, 1.0f, 0.6f)     // BonusLife (guessed green tone)
	};
	for (int type = PowerUp::Chaos; type < PowerUp::Type::End; ++type)
	{
		cumulativeWeight += PowerUp::s_Weights[type];
		if (cumulativeWeight >= chance)
		{
			glm::vec2 size = type == PowerUp::BonusLife ? glm::vec2(30.f) : PowerUp::s_Size;
			/*auto powerup = std::make_unique<PowerUp>(
				PowerUp::PowerTypeString(static_cast<PowerUp::Type>(type)),
				GetPowerupDuration(PWR_TYPE_INT(type)),
				obj.GetCenter() - glm::vec2(PowerUp::s_Size.x / 2.0f, PowerUp::s_Size.y / 2.0f),
				GET_TEXTURE(POWERUP_TEXTURE_NAMES[type]),
				POWERUP_COLORS[type],
				size
			);*/
			//m_PowerUps.push_back(powerup);
			break;
		}
	}
}

void Game::ResetLevel()
{
	for (auto* obj : m_Levels[m_Level]->GetEnt())
	{
		obj->m_Alive = true;
	}
}

void Game::ResetPlayer()
{
	glm::vec2 playerInitialPos = glm::vec2((m_Width - PLAYER_SIZE.x) / 2.0, m_Height - PLAYER_SIZE.y);
	glm::vec2 ballInitialPos{ playerInitialPos.x + PLAYER_SIZE.x / 2.0f - BALL_RADIUS,  playerInitialPos.y - 2.f * BALL_RADIUS };
	Player->m_Position = playerInitialPos;
	Ball->m_Position = ballInitialPos;
	Ball->m_Velocity = INITIAL_BALL_VELOCITY;
	Ball->m_Stuck = true;
}

bool Game::AnotherPowerupActive(std::string_view type)
{
	for (const auto& powerup : m_PowerUps)
	{
		if (type == powerup->m_Type)
		{
			return true;
		}
	}
	return false;
}

void PowerupDeactivate(std::string_view type)
{
	if (type == PWR_TYPE(Chaos))
	{
		Effects->m_Chaos = false;
	}
	else if (type == PWR_TYPE(Confuse))
	{
		Effects->m_Confuse = false;
	}
	else if (type == PWR_TYPE(DecreaseBallVelocity))
	{
		Ball->m_Velocity = INITIAL_BALL_VELOCITY;
	}
	else if (type == PWR_TYPE(IncreaseBallVelocity))
	{
		Ball->m_Velocity = INITIAL_BALL_VELOCITY;
	}
	else if (type == PWR_TYPE(StickyPaddle))
	{
		Ball->m_Sticky = false;
		Player->m_Color = glm::vec3(1.0f);
	}
	else if (type == PWR_TYPE(PassThrough))
	{
		Ball->m_PassThrough = false;
		Ball->m_Color = glm::vec3(1.0f);
	}
}

void Game::UpdatePowerUps()
{
	for (auto& powerup : m_PowerUps)
	{
		const char* type = powerup->m_Type;
		if (powerup->m_Alive)
		{
			powerup->m_Position += powerup->m_Velocity * m_DeltaTime;
		}
		if (powerup->m_Activated)
		{
			powerup->m_Duration -= m_DeltaTime;
			if (powerup->m_Duration <= 0)
			{
				powerup->m_Activated = false;

				if (AnotherPowerupActive(type))
					continue; // do not deactivate if another of the same type is active
				PowerupDeactivate(type);
				std::println("[Game] Power-up {} deactivated.", powerup->m_Type);
			}
		}
	}
}

void Game::UpdateDeltaTime()
{
	currentFrame = static_cast<float>(glfwGetTime());
	m_DeltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	// FPS counter
	counter++;
	if (currentFrame - fpsLastFrame >= 1.0f)
	{
		m_Window->SetTitle(
			std::format(
				"Breakout - FPS: {} | Level: {}",
				counter, m_Level
			).c_str());
		counter = 0;
		fpsLastFrame = currentFrame;
	}
}



void Game::ProcessInput()
{
	if (m_State == GameState::ACTIVE)
	{
		float velocity = m_DeltaTime * PLAYER_VELOCITY;
		if (m_Keys[GLFW_KEY_D] && Player->m_Position.x + Player->m_Size.x <= m_Width)
		{
			Player->m_Position.x += velocity;
			if (Ball->m_Stuck)
				Ball->m_Position.x += velocity;
		}
		else if (m_Keys[GLFW_KEY_A] && Player->m_Position.x >= 0.0f)
		{
			Player->m_Position.x -= velocity;
			if (Ball->m_Stuck)
				Ball->m_Position.x -= velocity;
		}

		//std::cout << Player->m_Position.x << " " << Player->m_Position.y << std::endl;

		if (m_Keys[GLFW_KEY_SPACE])
			Ball->m_Stuck = false;

		if (m_Keys[GLFW_KEY_ESCAPE])
		{
			WindowCloseEvent e{};
			this->OnCloseGame(e);
		}
	}
}

void Game::Update()
{
	Ball->Move(m_DeltaTime, m_Width);
	this->DoCollisions(*Ball);
	Particles->Update(m_DeltaTime, *Ball, 1);

	if (Effects->m_Shake)
	{
		ShakeTime -= m_DeltaTime;
		if (ShakeTime < 0)
			Effects->m_Shake = false;
	}

	if (Ball->m_Position.y >= this->m_Height) // did ball reach bottom edge?
	{
		m_Lives--;
		this->ResetPlayer();
	}

	if (m_Lives <= 0)
	{
		this->ResetLevel();
		this->ResetPlayer();
		m_State = GameState::GAMEOVER;
	}
}

void Game::Draw()
{
	if (m_State == GameState::ACTIVE || m_State == GameState::GAMEOVER)
	{
		// BACKGROUND IMAGE/OBJ FIRST
		renderer->DrawSprite(
			GET_TEXTURE("background"),
			glm::vec3(0.0f),
			0.0f,
			glm::vec2(m_Width, m_Height),
			glm::vec3(1.0));

		// PLAYER/PADDLE
		Player->Render(renderer);

		// LEVELS
		m_Levels[m_Level]->Draw(renderer);

		// PARTICLES
		Particles->Draw();
		// BALL
		Ball->Render(renderer);
	}
}


void Game::Run()
{
	while (m_State != GameState::TERMINATE)
	{

		this->UpdateDeltaTime();

		Effects->BeginRender();

		this->ProcessInput();
		this->Update();
		this->Draw();

		Effects->EndRender();
		Effects->RenderScreen(currentFrame);
		m_Window->SwapBuffers();
	}
}
