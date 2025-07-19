#pragma once
#include "Shader.h"
#include "Texture.h"
#include <string>
#include <string_view>
#include <map>

class ResourceManager
{
public:
	static Shader* LoadShader(std::string vPath, std::string fPath, std::string name);
	static Texture2D* LoadTexture(std::string path, std::string name);
	static Shader* GetShader(std::string name);
	static Texture2D* GetTexture(std::string name);
private:
	static bool ExtensionCheck(std::string_view str, const std::string checkStr);
	static Shader* LoadShaderFromFile(const char* vPath, const char* fPath);
	static Texture2D* LoadTextureFromFile(const char* data);

private:
	static inline std::map<std::string, Shader*> s_Shaders;
	static inline std::map<std::string, Texture2D*> s_Textures;
};

