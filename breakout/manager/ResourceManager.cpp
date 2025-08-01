#include "pch.h"
#include "ResourceManager.h"
#include "renderer/Texture.h"
#include <stb_image.h>


//std::map<std::string, Shader*> ResourceManager::s_Shaders;

Texture2D* ResourceManager::LoadTexture(std::string path, std::string name)
{
#ifdef _DEBUG
	bool expression{ ExtensionCheck(path, ".png") || ExtensionCheck(path, ".jpg") || ExtensionCheck(path, ".jpeg") };
#endif // _DEBUG
	assert(expression && "Wrong extension for vertex shader");
	assert(std::filesystem::exists(path) && "file doesn't exist");

	Texture2D* tex = LoadTextureFromFile(path.c_str());
	if (tex)
		s_Textures[name] = tex;
	else
		std::cerr << "Texture load failed\n";

	return tex;
}

Texture2D* ResourceManager::LoadTextureFromFile(const char* path)
{
	int width, height, channels;
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	Texture2D* texture{ nullptr };
	if (data)
	{
		if (channels == 4)
			texture = new Texture2D(width, height, GL_RGBA, GL_RGBA);
		else
			texture = new Texture2D(width, height);
		texture->Generate(width, height, data);
	}
	else
	{
		std::cerr << "Failed to load file: " << path << "\n";
	}
	stbi_image_free(data);
	return texture;
}

Texture2D* ResourceManager::GetTexture(std::string name)
{
	return s_Textures[name];
}

bool ResourceManager::ExtensionCheck(std::string_view str, const std::string checkStr)
{
	size_t pos = str.find_last_of('.');
	return pos != std::string::npos && str.substr(pos) == checkStr;
}

Shader* ResourceManager::LoadShaderFromFile(const char* vPath, const char* fPath)
{
	std::string vShaderStr, fShaderStr;
	std::ifstream vShaderFile, fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vPath);
		fShaderFile.open(fPath);

		std::stringstream ssVertShader, ssFragShader;
		ssVertShader << vShaderFile.rdbuf();
		ssFragShader << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vShaderStr = ssVertShader.str();
		fShaderStr = ssFragShader.str();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	const char* vShaderCode = vShaderStr.c_str();
	const char* fShaderCode = fShaderStr.c_str();

	Shader* shader = new Shader();
#ifdef _DEBUG
	assert(shader->Compile(vShaderCode, fShaderCode) && "Invalid Shader!!");
#else
	shader->Compile(vShaderCode, fShaderCode);
#endif // _DEBUG

	return shader;
}

Shader* ResourceManager::LoadShader(std::string vPath, std::string fPath, std::string name)
{
	assert(ExtensionCheck(vPath, ".vert") && "Wrong extension for vertex shader");
	assert(ExtensionCheck(fPath, ".frag") && "Wrong extension for fragment shader");
	assert(vPath != fPath && "Please provide correct paths");

	assert(std::filesystem::exists(vPath) && "vertex file doesn't exist");
	assert(std::filesystem::exists(fPath) && "fragment file doesn't exist");

	s_Shaders[name] = LoadShaderFromFile(vPath.c_str(), fPath.c_str());
	return s_Shaders[name];
}

Shader* ResourceManager::GetShader(std::string name)
{
	Shader* shader{ nullptr };
	try
	{
		shader = s_Shaders.at(name);
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "Shader not found: " << e.what() << "\n";
	}

	return shader;
}

void ResourceManager::Cleanup()
{
	std::print("[ResourceManager] Cleaning and deleting resources...\n");
	ITERATE_MAP_ERASE(s_Shaders)
	{
		delete it->second;
	}

	ITERATE_MAP_ERASE(s_Textures)
	{
		delete it->second;
	}
	std::print("[ResourceManager] Cleaned up.\n");
}