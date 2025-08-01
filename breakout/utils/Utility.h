#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

bool operator<=(glm::uvec2 a, glm::uvec2 b)
{
	return a.x <= b.x && a.y <= b.y;
}

bool operator<(glm::uvec2 a, glm::uvec2 b)
{
	return a.x < b.x && a.y < b.y;
}

bool operator==(glm::uvec2 a, glm::uvec2 b)
{
	return a.x == b.x && a.y == b.y;
}

bool operator>=(glm::uvec2 a, glm::uvec2 b)
{
	return a.x >= b.x && a.y >= b.y;
}

bool operator>(glm::uvec2 a, glm::uvec2 b)
{
	return a.x > b.x && a.y > b.y;
}

glm::uvec2 operator-(glm::uvec2 a, glm::uvec2 b)
{
	return { a.x - b.x, a.y - b.y };
}

glm::vec2 operator-(glm::vec2 a, glm::vec2 b)
{
	return { a.x - b.x, a.y - b.y };
}

glm::uvec2 operator+(glm::uvec2 a, glm::uvec2 b)
{
	return { a.x + b.x, a.y + b.y };
}

glm::vec2 operator+(glm::vec2 a, glm::vec2 b)
{
	return { a.x + b.x, a.y + b.y };
}