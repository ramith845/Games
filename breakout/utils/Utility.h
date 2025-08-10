#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


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

glm::vec2 operator/(glm::vec2 a, float num)
{
	return { a.x / num, a.y / num };
}

glm::vec2 operator*(glm::vec2 a, float num)
{
	return { a.x * num, a.y * num };
}
