#include "pch.h"
#include "ResourceManager.h"
#include "renderer/Texture.h"
#include "stbi/stb_image.h"

bool ResourceManager::ExtensionCheck(std::string_view str, const std::string checkStr)
{
	size_t pos = str.find_last_of('.');
	return pos != std::string::npos && str.substr(pos) == checkStr;
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture(std::string path, std::string name)
{
#ifdef _DEBUG
	bool expression{ ExtensionCheck(path, ".png") || ExtensionCheck(path, ".jpg") || ExtensionCheck(path, ".jpeg") };
#endif // _DEBUG
	assert(expression && "Wrong extension for vertex shader");
	assert(std::filesystem::exists(path) && "file doesn't exist");

	std::shared_ptr<Texture2D> tex = LoadTextureFromFile(path.c_str());
	if (tex)
		s_Textures[name] = tex;
	else
		std::cerr << "Texture load failed\n";

	return tex;
}

std::shared_ptr<Texture2D> ResourceManager::LoadTextureFromFile(const char* path)
{
	int width, height, channels;
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	std::shared_ptr<Texture2D> texture{ nullptr };
	if (data)
	{
		if (channels == 4)
			texture = std::make_shared<Texture2D>(width, height, GL_RGBA, GL_RGBA);
		else
			texture = std::make_shared<Texture2D>(width, height);
		texture->Generate(width, height, data);
	}
	else
	{
		std::cerr << "Failed to load file: " << path << "\n";
	}
	stbi_image_free(data);
	return texture;
}

std::shared_ptr<Texture2D> ResourceManager::GetTexture(std::string name)
{
	auto it = s_Textures.find(name);
	if (it != s_Textures.end())
		return it->second;
	return nullptr;
}


std::shared_ptr<Shader> ResourceManager::LoadShaderFromFile(const char* vPath, const char* fPath)
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

	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
#ifdef _DEBUG
	assert(shader->Compile(vShaderCode, fShaderCode) && "Invalid Shader!!");
#else
	shader->Compile(vShaderCode, fShaderCode);
#endif // _DEBUG

	return shader;
}

std::shared_ptr<Shader> ResourceManager::LoadShader(std::string vPath, std::string fPath, std::string name)
{
    if (vPath.rfind("shaders/", 0) != 0)
        vPath = "shaders/" + vPath;
    if (fPath.rfind("shaders/", 0) != 0)
        fPath = "shaders/" + fPath;
	assert(ExtensionCheck(vPath, ".vert") && "Wrong extension for vertex shader");
	assert(ExtensionCheck(fPath, ".frag") && "Wrong extension for fragment shader");
	assert(vPath != fPath && "Please provide correct paths");

	assert(std::filesystem::exists(vPath) && "vertex file doesn't exist");
	assert(std::filesystem::exists(fPath) && "fragment file doesn't exist");

	s_Shaders[name] = LoadShaderFromFile(vPath.c_str(), fPath.c_str());
	return s_Shaders[name];
}

std::shared_ptr<Shader> ResourceManager::GetShader(std::string name)
{
	std::shared_ptr<Shader> shader{ nullptr };
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
	s_Shaders.clear();
	s_Textures.clear();
}