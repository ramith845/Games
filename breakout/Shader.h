#pragma once
#include "glm.hpp"

class Shader
{
public:
	enum class Type
	{
		VERTEX,
		GEOMETRY,
		FRAGMENT,
		PROGRAM
	};
	Shader();
	~Shader();

	bool Compile(const char* vertexCodeSrc, const char* fragmentCodeSrc, const char* geometryShaderSrc = nullptr);
	void use() const;

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, glm::vec3 color) const;
	void SetMatrix4(const std::string& name, glm::mat4 mat) const;
private:
	bool CheckCompileErrors(unsigned int ID, Type type) const;
private:
	unsigned int m_ID;
};
