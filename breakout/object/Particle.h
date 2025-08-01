#pragma once
#include "glm.hpp"

struct Particle
{
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec2 Size;
	glm::vec4 Color;
	float Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f), Size(10.0f) {
	}
};

