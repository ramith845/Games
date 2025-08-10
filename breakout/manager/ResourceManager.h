#pragma once
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include <string>
#include <string_view>
#include <map>

class ResourceManager
{
public:
    static std::shared_ptr<Shader> LoadShader(std::string vPath, std::string fPath, std::string name);
    static std::shared_ptr<Texture2D> LoadTexture(std::string path, std::string name);
    static std::shared_ptr<Shader> GetShader(std::string name);
    static std::shared_ptr<Texture2D> GetTexture(std::string name);
    static void Cleanup();
private:
    static bool ExtensionCheck(std::string_view str, const std::string checkStr);
    static std::shared_ptr<Shader> LoadShaderFromFile(const char* vPath, const char* fPath);
    static std::shared_ptr<Texture2D> LoadTextureFromFile(const char* data);

private:
    static inline std::map<std::string, std::shared_ptr<Shader>> s_Shaders;
    static inline std::map<std::string, std::shared_ptr<Texture2D>> s_Textures;
};

