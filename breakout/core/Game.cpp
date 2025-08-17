#include "pch.h"
#include "Game.h"
#include "level/GameLevel.h"
#include "object/PowerUp.h"
#include "object/GameObject.h"
#include "object/BallObject.h"
#include "manager/ParticleGenerator.h"
#include "manager/ResourceManager.h"
#include "manager/CollisionManager.h"
#include "renderer/SpriteRenderer.h"
#include "renderer/CollisionDebugSpriteRenderer.h"
#include "renderer/PostProcessor.h"
#include "utils/Random.h"
#include "utils/Utility.h"
#include "Globals.h"
#include "irrKlang/irrKlang.h"

SpriteRenderer* renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;
PostProcessor* Effects;

//CollisionDebugSpriteRenderer* collisionBoxRenderer;
//GameObject* collisionBox1;
//GameObject* collisionBox2;
//std::vector<GameObject*> debugBoxes;
//GameObject* selected{ nullptr };
//CollisionManager::MovableObj_t movableObj{ std::nullopt };
CollisionManager cm;

void print_game_object_info(const char* label, const GameObject* obj);
//void init_collision_boxes(glm::mat4 projection);
void load_assets(glm::mat4 projection);
void load_all_levels(Game& game);
bool contains_point(glm::vec2 pos, BoundinBox bb);
Game::IrrKlangEnginePtr_t create_sound_engine();

Game::Game(unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height), m_Level(0)
	, m_State(GameState::ACTIVE), m_Keys(), m_DeltaTime(0.f)
{
	if (width < 0 || height < 0) {
		throw std::invalid_argument("Width and height must be non-negative.");
	}

	PowerUp::s_Weights[PowerUp::Chaos] = 30;
	PowerUp::s_Weights[PowerUp::Confuse] = 25;
	PowerUp::s_Weights[PowerUp::DecreasePaddleSize] = 22;
	PowerUp::s_Weights[PowerUp::DecreaseBallVelocity] = 18;
	PowerUp::s_Weights[PowerUp::IncreaseBallVelocity] = 14;
	PowerUp::s_Weights[PowerUp::PassThrough] = 10;
	PowerUp::s_Weights[PowerUp::StickyPaddle] = 8;
	PowerUp::s_Weights[PowerUp::IncreasePaddleSize] = 6;
	PowerUp::s_Weights[PowerUp::BonusLife] = 2;

	PowerUp::s_TotalWeight = 0;
	for (int i = 0; i < PowerUp::End; i++)
	{
		PowerUp::s_TotalWeight += PowerUp::s_Weights[i];
	}

	m_Window = std::make_unique<Window>(width, height, "Game");
	if (!m_Window->IsSuccess())
	{
		throw std::runtime_error("Failed to create window!");
	}
	m_Window->SetCBEventfn(std::bind(&Game::OnEvent, this, std::placeholders::_1));
	m_SoundEngine = create_sound_engine();
	m_SoundEngine->loadPlugins("ikpMP3.dll");
	//irrklang::ISound* music = m_SoundEngine->play2D(SONG_PATH, LOOP, START_UNPAUSED, TRACK/*returns sound for tweaking*/);
	//if (music) music->setVolume(0.2f);
}

Game::Game(int width, int height)
	: Game(static_cast<unsigned int>(width), static_cast<unsigned int>(height))
{

}

Game::~Game()
{
	delete renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
	/*delete collisionBoxRenderer;
	delete collisionBox1;
	delete collisionBox2;*/

	ResourceManager::Cleanup();

	std::println("[Game] Cleanup complete.");
}


int Game::Init()
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);
	load_assets(projection);

	load_all_levels(*this);

	InitPlayer();

	//init_collision_boxes(projection);

	return 1;
}

//void init_collision_boxes(glm::mat4 projection)
//{
//	ResourceManager::LoadShader("3.3.collision_shader.vert", "3.3.collision_shader.frag", "collision_shader");
//	GET_SHADER("collision_shader")->use()->SetMatrix4("projection", projection);
//
//	collisionBoxRenderer = new CollisionDebugSpriteRenderer(GET_SHADER("collision_shader"));
//	collisionBox1 = new BallObject(glm::vec2{ 20.f, 50.f }, 50.f, nullptr);
//	collisionBox2 = new BallObject(glm::vec2{ 120.f, 50.f }, 50.f, nullptr);
//	debugBoxes.push_back(collisionBox1);
//	debugBoxes.push_back(collisionBox2);
//}

void load_assets(glm::mat4 projection)
{
	// SPRITE SHADER LOADING
	ResourceManager::LoadShader("shaders/3.3.shader.vert", "shaders/3.3.shader.frag", "sprite_shader");
	GET_SHADER("sprite_shader")->use();
	GET_SHADER("sprite_shader")->SetInt("image", 0);
	GET_SHADER("sprite_shader")->SetMatrix4("projection", projection);

	renderer = new SpriteRenderer(GET_SHADER("sprite_shader"));

	// PARTICLE SHADER LOADING
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
		ResourceManager::LoadTexture(POWERUP_TEXTURES[i].path, POWERUP_TEXTURES[i].name);
	}
}

void load_all_levels(Game& game)
{
	std::vector<const char*> levelPaths{ "levels/one.lvl", "levels/two.lvl", "levels/three.lvl", "levels/four.lvl" };
	for (const char* path : levelPaths) {
		auto level = std::make_unique<GameLevel>();
		level->LoadLevel(path, game.m_Width, game.m_Height / 2);
		game.m_Levels.push_back(std::move(level));
	}
	game.m_Level = 0;
}

Game::IrrKlangEnginePtr_t create_sound_engine()
{
	return Game::IrrKlangEnginePtr_t(
		irrklang::createIrrKlangDevice(),
		[](irrklang::ISoundEngine* engine)
		{
			if (engine)
				engine->drop();
		});
}

void Game::InitPlayer()
{
	glm::vec2 playerInitialPos = glm::vec2(383.7f, m_Height - INITIAL_PLAYER_SIZE.y);
	Player = new GameObject{
		playerInitialPos,
		INITIAL_PLAYER_SIZE,
		GET_TEXTURE("paddle"),
		ObjectType::Player,
		glm::vec3(1.0),
		glm::vec2(PLAYER_VELOCITY) };
	m_InitPlayer = std::make_unique<GameObject>(*Player);

	Texture2DPtr t = GET_TEXTURE("face");

	glm::vec2 ballInitialPos{ playerInitialPos.x + INITIAL_PLAYER_SIZE.x / 2.0f - BALL_RADIUS,  playerInitialPos.y - 2.f * BALL_RADIUS };
	Ball = new BallObject(ballInitialPos, BALL_RADIUS, GET_TEXTURE("face"), INITIAL_BALL_VELOCITY);
	m_InitBall = std::make_unique<BallObject>(*Ball);

	Particles = new ParticleGenerator(GET_SHADER("particle_shader"), GET_TEXTURE("particle"), 500);

	Effects = new PostProcessor(m_Width, m_Height);
}

void Game::OnEvent(Event& e)
{
	using type = decltype(Game::OnKeyPress);
	EventDispatcher handler{ e };
	handler.Dispatch<KeyPressEvent>(std::bind(&Game::OnKeyPress, this, std::placeholders::_1));
	handler.Dispatch<KeyReleaseEvent>(std::bind(&Game::OnKeyRelease, this, std::placeholders::_1));
	handler.Dispatch<WindowCloseEvent>(std::bind(&Game::OnCloseGame, this, std::placeholders::_1));
	handler.Dispatch<WindowResizeEvent>(std::bind(&Game::OnResizeGame, this, std::placeholders::_1));
	handler.Dispatch<MouseMovedEvent>(std::bind(&Game::OnMouseMove, this, std::placeholders::_1));
	handler.Dispatch<MouseBtnReleasedEvent>(std::bind(&Game::OnMouseBtnRelease, this, std::placeholders::_1));
	handler.Dispatch<MouseBtnPressedEvent>(std::bind(&Game::OnMouseBtnPress, this, std::placeholders::_1));
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
	//std::println("Key release: {}", (char)e.GetKeyCode());
	return true;
}

bool Game::OnKeyPress(KeyPressEvent& e)
{
	m_Keys[static_cast<size_t>(e.GetKeyCode())] = true;
	std::println("Key press: {}", (char)e.GetKeyCode());
	return true;
}

bool Game::OnMouseMove(MouseMovedEvent& e)
{
	m_Mouse.x = e.GetX();
	m_Mouse.y = e.GetY();
	//std::println("{}", e.ToString());
	return true;
}

bool Game::OnMouseBtnRelease(MouseBtnReleasedEvent& e)
{
	m_IsDragging = false;
	std::println("{}", e.ToString());
	/*if (selected)
		movableObj = std::reference_wrapper<GameObject>(*selected);
	selected = nullptr;*/
	return true;
}

bool Game::OnMouseBtnPress(MouseBtnPressedEvent& e)
{
	m_IsDragging = true;
	//CheckWithinCollisionBox();
	std::println("{}", e.ToString());
	return true;
}

float get_powerup_duration(std::string_view type)
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
		return 10.0f;
	else if (type == PWR_TYPE(PassThrough))
		return 10.0f;
	else if (type == PWR_TYPE(BonusLife))
		return 0.0f;
	std::println("Unknown powerup encountered");
	return 5.0f;
}

void Game::SpawnPowerups(GameObject& obj)
{
	int bias = PowerUp::s_TotalWeight * 2;
	int chance = Random::get(0, PowerUp::s_TotalWeight + bias);
	int cumulativeWeight = 0;
	//Iterate through the powerup types and their weights
	for (int type = PowerUp::Chaos; type < PowerUp::Type::End; ++type)
	{
		cumulativeWeight += PowerUp::s_Weights[type];
		if (cumulativeWeight >= chance)
		{
			glm::vec2 size = (type == PowerUp::BonusLife) ? glm::vec2(30.f) : PowerUp::s_Size;
			auto powerup = std::make_unique<PowerUp>(
				PowerUp::PowerTypeString(static_cast<PowerUp::Type>(type)).data(),
				get_powerup_duration(PWR_TYPE_INT(type)),
				obj.GetCenter() - PowerUp::s_Size / 2.0f,
				GET_TEXTURE(POWERUP_TEXTURES[type].name),
				POWERUP_TEXTURES[type].color,
				size
			);
			m_PowerUps.push_back(std::move(powerup));
			break;
		}
	}
}

void activate_power_up(PowerUp& powerup, Game& game)
{
	if (powerup.m_Activated)
		return;
	powerup.m_Activated = true;
	std::println("Activating powerup: {}", powerup.m_Type);
	if (powerup.m_Type == PWR_TYPE(Chaos))
	{
		Effects->m_Chaos = true;
	}
	else if (powerup.m_Type == PWR_TYPE(DecreasePaddleSize))
	{
		auto fixedCenter = Player->GetCenter().x;
		auto newBallRelOffset = (Ball->GetCenter().x - fixedCenter) / PLAYER_SIZE_PWRUP_FACTOR;
		Player->m_Size.x = INITIAL_PLAYER_SIZE.x / PLAYER_SIZE_PWRUP_FACTOR;
		Player->SetPosX(fixedCenter - Player->m_Size.x / 2.0f);
		if (Ball->m_Stuck)
		{
			Ball->SetPosX(fixedCenter + newBallRelOffset - Ball->m_Radius);
		}
	}
	else if (powerup.m_Type == PWR_TYPE(IncreaseBallVelocity))
	{
		float speed = glm::length(INITIAL_BALL_VELOCITY) * BALL_SIZE_PWRUP_FACTOR;
		glm::vec2 direction = glm::normalize(Ball->m_Velocity);
		Ball->m_Velocity = speed * direction;
	}
	else if (powerup.m_Type == PWR_TYPE(Confuse))
	{
		Effects->m_Confuse = true;
	}
	else if (powerup.m_Type == PWR_TYPE(DecreaseBallVelocity))
	{
		float speed = glm::length(INITIAL_BALL_VELOCITY) / BALL_SIZE_PWRUP_FACTOR;
		glm::vec2 direction = glm::normalize(Ball->m_Velocity);
		Ball->m_Velocity = speed * direction;
	}
	else if (powerup.m_Type == PWR_TYPE(IncreasePaddleSize))
	{
		auto fixedCenter = Player->GetCenter().x;
		auto newBallRelOffset = (Ball->GetCenter().x - fixedCenter) * PLAYER_SIZE_PWRUP_FACTOR;
		Player->m_Size.x = INITIAL_PLAYER_SIZE.x * PLAYER_SIZE_PWRUP_FACTOR;
		Player->SetPosX(fixedCenter - Player->m_Size.x / 2.0f);
		if (Ball->m_Stuck)
		{
			Ball->SetPosX(fixedCenter + newBallRelOffset - Ball->m_Radius);
		}
	}
	else if (powerup.m_Type == PWR_TYPE(StickyPaddle))
	{
		Ball->m_Sticky = true;
	}
	else if (powerup.m_Type == PWR_TYPE(PassThrough))
	{
		Ball->m_PassThrough = true;
		Ball->m_Color = glm::vec3(0.98f, 0.53f, 0.53f);
	}
	else if (powerup.m_Type == PWR_TYPE(BonusLife))
	{
		game.m_Lives++;
	}
	powerup.m_Duration = get_powerup_duration(powerup.m_Type);
} // activate_power_up

glm::vec2 clamp(glm::vec2 val, glm::vec2 min, glm::vec2 max)
{
	return
	{
		std::max(min.x, std::min(max.x, val.x)),
		std::max(min.y, std::min(max.y, val.y))
	};
}

Direction collision_direction(glm::vec2 vec)
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

Collision_t check_collision(BallObject& one, GameObject& two)
{
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
		return std::make_tuple(true, collision_direction(nearestVec), nearestVec);
	}
	else
	{
		return std::make_tuple(false, UP, glm::vec2(0.0f));
	}
}

bool check_collision(GameObject& one, GameObject& two)
{
	// square based collision check AABB - naive approach for any shape
	return one.m_bb.min <= two.m_bb.max && two.m_bb.min <= one.m_bb.max; // overloaded op<=
}

bool handle_brick_collision(Game& game, BallObject& ball, GameObject* box)
{
	CollisionManager::MovableObj_t movableObj = std::ref(ball);
	// circle vs AABB collision check,
	CollisionManager::SATResult_t collision = cm.SATCollisionTest(*box, ball, movableObj);
	if (std::get<0>(collision))
	{
		if (!box->m_Solid)
		{
			game.m_SoundEngine->play2D(BLEEP_SFX_PATH, NO_LOOP);
			box->m_Alive = false;
			game.SpawnPowerups(*box);
		}
		else
		{
			game.m_SoundEngine->play2D(SOLID_SFX_PATH, NO_LOOP);
			ShakeTime = 0.05f;
			Effects->m_Shake = true;
		}

		// handle collision response
		Direction dir = collision_direction(std::get<1>(collision));
		// skip if pass through and breakable box, otherwise reflect
		if (!(Ball->m_PassThrough && !box->m_Solid))
		{
			if (dir == LEFT || dir == RIGHT)
			{
				ball.m_Velocity.x *= -1;
			}
			else
			{
				ball.m_Velocity.y *= -1;
			}
		}
		return true;
	}
	return false;
}

void Game::DoCollisions(BallObject& ball)
{
	// ball with all boxes in the current level
	for (GameObject* box : m_Levels[m_Level]->GetEnt())
	{
		if (!box->m_Alive)
			continue;

		// one collision only even for two+
		if (handle_brick_collision(*this, ball, box))
		{
			break;
		}
	}

	// ball with player paddle
	if (!Ball->m_Stuck)
	{
		Collision_t playerCollision = check_collision(ball, *Player);
		if (std::get<0>(playerCollision) && std::get<1>(playerCollision) == UP)
		{
			m_SoundEngine->play2D(BLEEP1_SFX_PATH, NO_LOOP);
			glm::vec2 CPlaya = Player->GetCenter();
			glm::vec2 CBall = ball.GetCenter();
			float distance = CBall.x - CPlaya.x;
			float per = distance / (Player->m_Size.x / 2.0f);
			float strength = 2.0f;
			glm::vec2 oldVelocty = ball.m_Velocity;
			ball.m_Velocity.x = strength * per * INITIAL_BALL_VELOCITY.x;
			ball.m_Velocity.y = -1.f * abs(ball.m_Velocity.y);
			ball.m_Velocity = glm::normalize(ball.m_Velocity) * glm::length(oldVelocty);
			Ball->m_Stuck = Ball->m_Sticky;
			//print_game_object_info("Ball", &ball);
			//print_game_object_info("CollidedWith Player: ", Player);
		}
	}

	// player with powerup objects
	for (auto& powerup : m_PowerUps)
	{
		if (powerup->m_Alive)
		{
			powerup->SetPos(powerup->GetPos()); // triggers bounding box update
			Player->SetPos(Player->GetPos()); // triggers bounding box update
			if (check_collision(*Player, *powerup.get()))
			{
				m_SoundEngine->play2D(POWERUP_SFX_PATH, NO_LOOP);
				powerup->m_Alive = false;
				activate_power_up(*powerup, *this);
			}
		}
	}
}

void Game::ResetLevel()
{
	m_Lives = 3;
	for (auto* obj : m_Levels[m_Level]->GetEnt())
	{
		obj->m_Alive = true;
	}

	m_PowerUps.clear();
	// Reset PLAYER, BALL and EFFECTS DATA 
	*Ball = *m_InitBall;
	*Player = *m_InitPlayer;

	Effects->m_Chaos = false;
	Effects->m_Confuse = false;
	Effects->m_Shake = false;
}

void Game::ResetPlayer()
{
	glm::vec2 playerInitialPos = glm::vec2((m_Width - INITIAL_PLAYER_SIZE.x) / 2.0, m_Height - INITIAL_PLAYER_SIZE.y);
	glm::vec2 ballInitialPos{ playerInitialPos.x + INITIAL_PLAYER_SIZE.x / 2.0f - BALL_RADIUS,  playerInitialPos.y - 2.f * BALL_RADIUS };
	Player->m_Size = INITIAL_PLAYER_SIZE;
	Player->SetPos(playerInitialPos);
	Ball->SetPos(ballInitialPos);
	Ball->m_Velocity = INITIAL_BALL_VELOCITY;
	Ball->m_Stuck = true;
}

bool another_powerup_active(std::string_view type, const std::vector<std::unique_ptr<PowerUp>>& powerups)
{
	for (const auto& powerup : powerups)
	{
		if (type == powerup->m_Type && powerup->m_Activated)
		{
			return true;
		}
	}
	return false;
}

template<typename T>
void Game::PowerupDeactivate(T& powerup)
{
	if (powerup->m_Duration <= 0)
	{
		powerup->m_Activated = false;
		std::string_view type = powerup->m_Type;
		if (another_powerup_active(type, m_PowerUps))
			return; // do not deactivate if another of the same type is active

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
			float speed = glm::length(INITIAL_BALL_VELOCITY);
			glm::vec2 direction = glm::normalize(Ball->m_Velocity);
			Ball->m_Velocity = speed * direction;
		}
		else if (type == PWR_TYPE(IncreaseBallVelocity))
		{
			float speed = glm::length(INITIAL_BALL_VELOCITY);
			glm::vec2 direction = glm::normalize(Ball->m_Velocity);
			Ball->m_Velocity = speed * direction;
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
		std::println("[Game] Power-up {} deactivated.", powerup->m_Type);
	}
}

void Game::UpdatePowerUps()
{
	for (auto& powerup : m_PowerUps)
	{
		//auto p = powerup.get();
		if (powerup->m_Alive)
		{
			powerup->SetPos(powerup->GetPos() + powerup->m_Velocity * m_DeltaTime);

		}
		if (powerup->m_Activated)
		{
			powerup->m_Duration -= m_DeltaTime;
			PowerupDeactivate(powerup);
		}
	}

	//// Remove inactive power-ups
	m_PowerUps.erase(
		// returns ptr to start of start
		std::remove_if( // moves unwanted ele to back based on lambda predicate
			m_PowerUps.begin(), m_PowerUps.end(),
			[](const auto& powerup)
			{
				//if (!(powerup->m_Alive || powerup->m_Activated))
					//std::println("destroyed powerup: {}", powerup->m_Type);
				return !(powerup->m_Alive || powerup->m_Activated);
			}
		),
		m_PowerUps.end()
	);
}

bool contains_point(glm::vec2 pos, BoundinBox bb)
{
	return bb.min <= pos && pos <= bb.max;
}

/*void Game::CheckWithinCollisionBox()
{
	m_DragVector.initialPos = glm::vec2(m_Mouse.x, m_Mouse.y);
	for (auto* box : std::ranges::reverse_view(debugBoxes))
		if (contains_point(m_DragVector.initialPos, box->m_bb))
		{
			selected = box;
			m_DragVector.centerPos = box->GetCenter();
			return;
		}
	selected = nullptr;
	m_DragVector.centerPos = m_DragVector.initialPos;
}*/

void Game::Update()
{
	Ball->Move(m_DeltaTime, m_Width);
	this->DoCollisions(*Ball);

	Particles->Update(m_DeltaTime, *Ball, 1);

	if (m_PowerUps.size() > 0)
		this->UpdatePowerUps();

	if (Effects->m_Shake)
	{
		ShakeTime -= m_DeltaTime;
		if (ShakeTime < 0)
			Effects->m_Shake = false;
	}

	if (Ball->GetPos().y >= this->m_Height) // did ball reach bottom edge?
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

	for (const auto& p : m_PowerUps)
	{
		if (p->m_Alive && p->GetPos().y >= m_Height)
		{
			p->m_Alive = false;
		}
	}

	/*if (m_IsDragging && selected)
	{
		glm::vec2 obj_center = m_Mouse + m_DragVector.GetOffset();
		selected->SetPos(obj_center - selected->m_Size / 2.0f);
	}*/

	/*CollisionManager cm;
	CollisionManager::SATResult_t result = cm.SATCollisionTest(*collisionBox1, *collisionBox2, movableObj);
	if (std::get<0>(result) && movableObj)
	{
		std::println("movableObj: not nullopt");
		movableObj->get().m_Color = glm::vec3(0.0f, 1.0f, 0.0f);
	}*/
}

void Game::UpdateDeltaTime()
{
	currentFrame = static_cast<float>(glfwGetTime());
	m_DeltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	// FPS counter
	//debug_frame++;
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
	/*if (m_BrickCollision)
	{
		m_DeltaTime = 0;
	}*/
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

		// LEVELS
		m_Levels[m_Level]->Draw(renderer);

		// PLAYER/PADDLE
		Player->Render(renderer);

		// PARTICLES
		Particles->Draw();
		// BALL
		Ball->Render(renderer);

		// POWERUPS
		for (const auto& powerup : m_PowerUps)
		{
			if (powerup->m_Alive)
			{
				powerup->Render(renderer);
			}
		}

		// dont imclude in commit only debug purpose not a feature
		/*for (const auto* box : debugBoxes)
		{
			if (!box) continue;
			auto ball = dynamic_cast<const BallObject*>(box);
			if (ball)
				const_cast<BallObject*>(ball)->Render(collisionBoxRenderer);
			else
				box->Render(collisionBoxRenderer);
		}*/
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

void Game::ProcessInput()
{
	if (m_State == GameState::ACTIVE)
	{
		float velocity = m_DeltaTime * PLAYER_VELOCITY;
		if (m_Keys[GLFW_KEY_D] && Player->GetPos().x + Player->m_Size.x <= m_Width)
		{
			Player->SetPosX(Player->GetPos().x + velocity);
			if (Ball->m_Stuck)
				Ball->SetPosX(Ball->GetPos().x + velocity);
		}
		else if (m_Keys[GLFW_KEY_A] && Player->GetPos().x >= 0.0f)
		{
			Player->SetPosX(Player->GetPos().x - velocity);
			if (Ball->m_Stuck)
				Ball->SetPosX(Ball->GetPos().x - velocity);
		}

		//std::cout << Player->m_Position.x << " " << Player->m_Position.y << std::endl;

		if (m_Keys[GLFW_KEY_SPACE])
			Ball->m_Stuck = false;

		if (m_Keys[GLFW_KEY_ESCAPE])
		{
			WindowCloseEvent e{};
			this->OnCloseGame(e);
			//__debugbreak();
			this;
		}

		/*if (m_Keys[GLFW_KEY_ENTER])
		{
			m_BrickCollision = false;
		}*/
	}
}

// Add this helper function at the top of Game.cpp or in an appropriate utility location
void print_game_object_info(const char* label, const GameObject* obj) {
	std::println("{}: pos=({}, {}), size=({}, {}), bb=({}, {})-({}, {}), id={}, type={}",
		label,
		obj->m_Alive ? "Alive" : "Dead",
		obj->GetPos().x, obj->GetPos().y,
		obj->m_Size.x, obj->m_Size.y,
		obj->m_bb.min.x, obj->m_bb.min.y, obj->m_bb.max.x, obj->m_bb.max.y,
		obj->GetID(),
		typeid(*obj).name()
	);
}

