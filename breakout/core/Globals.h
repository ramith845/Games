#pragma once

constexpr const bool LOOP = true;
constexpr const bool NO_LOOP = false;
constexpr const bool START_UNPAUSED = false;
constexpr const bool TRACK = true;
constexpr const char* SONG_PATH = "resources/getout.ogg";
constexpr const char* PING_SFX_PATH = "resources/ping-82822.mp3";
constexpr const char* BLEEP_SFX_PATH = "resources/bleep.mp3";
constexpr const char* BLEEP1_SFX_PATH = "resources/bleep.wav";
constexpr const char* SOLID_SFX_PATH = "resources/solid.wav";
constexpr const char* POWERUP_SFX_PATH = "resources/powerup.wav";

constexpr const char* BACKGROUND_IMG = "resources/background1_edit.jpg";
constexpr const char* AWESOMEFACE_IMG = "resources/awesomeface.png";
constexpr const char* BLOCK_IMG = "resources/block.png";
constexpr const char* BLOCK_SOLID_IMG = "resources/block_solid.png";
constexpr const char* PADDLE_IMG = "resources/paddle.png";
constexpr const char* PARTICLE_IMG = "resources/particle.png";

struct PowerupTexture
{
	PowerUp::Type type;
	const char* path;
	const char* name;
	glm::vec3 color;
};
constexpr PowerupTexture POWERUP_TEXTURES[] = {
	{ PowerUp::Chaos, "resources/powerup_chaos.png", "powerup_chaos", glm::vec3(0.9f, 0.25f, 0.25f) },
	{ PowerUp::DecreasePaddleSize, "resources/powerup_decrease.png", "powerup_decrease", glm::vec3(1.0f, 0.6f, 0.4f) },
	{ PowerUp::IncreaseBallVelocity, "resources/powerup_speed.png", "powerup_speed", glm::vec3(0.5f, 0.5f, 1.0f) },
	{ PowerUp::Confuse, "resources/powerup_confuse.png", "powerup_confuse", glm::vec3(1.0f, 0.3f, 0.3f) },
	{ PowerUp::DecreaseBallVelocity, "resources/powerup_slow.png", "powerup_slow", glm::vec3(0.3f, 0.3f, 1.0f) },
	{ PowerUp::IncreasePaddleSize, "resources/powerup_increase.png", "powerup_increase", glm::vec3(1.0f, 0.6f, 0.4f) },
	{ PowerUp::StickyPaddle, "resources/powerup_sticky.png", "powerup_sticky", glm::vec3(1.0f, 0.5f, 1.0f) },
	{ PowerUp::PassThrough, "resources/powerup_passthrough.png", "powerup_passthrough", glm::vec3(0.5f, 1.0f, 0.5f) },
	{ PowerUp::BonusLife, "resources/powerup_life.png", "powerup_bonus_life", glm::vec3(0.6f, 1.0f, 0.6f) }
};


const float PLAYER_VELOCITY(250.0f);
const glm::vec2 INITIAL_PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_SIZE_PWRUP_FACTOR{ 2.f };

const float BALL_SIZE_PWRUP_FACTOR{ 1.5f };
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f); // intial meaning it can change later
const float BALL_RADIUS = 12.5f;


static float lastFrame = 0.0f;
static float currentFrame;
static float fpsLastFrame = 0.0f;
static int counter{ 0 };
static size_t debug_frame{ 0 };
static float ShakeTime = 0.0f;
