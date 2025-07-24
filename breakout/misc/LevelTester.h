#pragma once
#include "level/GameLevel.h"
namespace LevelTester
{
	using fn_t = std::function<void(const char*)>;

	int Driver(fn_t ExampleLoader);
}
