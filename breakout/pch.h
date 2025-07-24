#pragma once
#include "Windows.h"
#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/matrix_transform.hpp"


#include <io.h>
#include <cstdio>
#include <iostream>
#include <print>
#include <iomanip>
#include <format>
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>
#include <string_view>
#include <filesystem>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <ranges>
#include <random>
#include <functional>
#include <utility>
#include <limits>
#include <type_traits>

#define GET_SHADER(x) ResourceManager::GetShader(x)
#define GET_TEXTURE(x) ResourceManager::GetTexture(x)

#define REMOVE_PARENS(X) REMOVE_PARENS_HELPER X
#define REMOVE_PARENS_HELPER(...) __VA_ARGS__

#define ITERATE_MAP(x) \
for (decltype(x)::const_iterator it = x.begin(); it != x.end(); ++it)

#define ITERATE_MAP_ERASE(x) \
for (decltype(x)::const_iterator it = x.begin(); it != x.end();it = x.erase(it))