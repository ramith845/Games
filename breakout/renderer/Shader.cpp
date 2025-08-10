#include "pch.h"
#include "Shader.h"

Shader::Shader()
{
	m_ID = glCreateProgram();
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
	if (m_VertexCodeSrc)
	{
		delete[] m_VertexCodeSrc;
		m_VertexCodeSrc = nullptr;
	}
	if (m_FragmentCodeSrc)
	{
		delete[] m_FragmentCodeSrc;
		m_FragmentCodeSrc = nullptr;
	}
}

bool CopyShaderSource(const char* src, const char** dest)
{
	*dest = new char[strlen(src) + 1];
	errno_t result = strcpy_s(const_cast<char*>(*dest), strlen(src) + 1, src);
	if (result != 0) {
		std::cerr << "[Shader][ERROR] Failed to copy shader src\n";
		return false;
	}
	return true;
}
bool Shader::Compile(const char* vertexCodeSrc, const char* fragmentCodeSrc, const char* geometryShaderSrc)
{
	if (!vertexCodeSrc || !fragmentCodeSrc
		|| !CopyShaderSource(vertexCodeSrc, &m_VertexCodeSrc)
		|| !CopyShaderSource(fragmentCodeSrc, &m_FragmentCodeSrc)
		)
	{
		std::cerr << "[Shader] Error: Vertex and Fragment shader source code must be provided." << std::endl;
		return false;
	}


	bool compiled{ true };
	unsigned int vShader{};
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vertexCodeSrc, NULL);
	glCompileShader(vShader);
	compiled &= CheckCompileErrors(vShader, Type::VERTEX);

	if (geometryShaderSrc)
	{
		unsigned int gShader{};
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometryShaderSrc, NULL);
		glCompileShader(gShader);
		compiled &= CheckCompileErrors(gShader, Type::GEOMETRY);
	}

	unsigned int fShader{};
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fragmentCodeSrc, NULL);
	glCompileShader(fShader);
	compiled &= CheckCompileErrors(fShader, Type::FRAGMENT);

	glAttachShader(m_ID, vShader);
	glAttachShader(m_ID, fShader);
	glLinkProgram(m_ID);
	compiled &= CheckCompileErrors(m_ID, Type::PROGRAM);

	return compiled;
}

const Shader* Shader::use() const
{
	glUseProgram(m_ID);
	return this;
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::Set1iv(const std::string& name, int count, int* value) const
{
	glUniform1iv(glGetUniformLocation(m_ID, name.c_str()), count, value);
}

void Shader::Set1fv(const std::string& name, int count, float* value) const
{
	glUniform1fv(glGetUniformLocation(m_ID, name.c_str()), count, value);
}

void  Shader::SetVec2(const std::string& name, glm::vec2 color) const
{
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), color.x, color.y);
}

void Shader::SetVec2fv(const std::string& name, int count, float* value) const
{
	glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), count, value);
}

void  Shader::SetVec3(const std::string& name, glm::vec3 color) const
{
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), color.x, color.y, color.z);
}

void  Shader::SetVec4(const std::string& name, glm::vec4 color) const
{
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), color.x, color.y, color.z, color.w);
}

void  Shader::SetMatrix4(const std::string& name, glm::mat4 mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, false, glm::value_ptr(mat));
}

bool Shader::CheckCompileErrors(unsigned int id, Type type) const
{
	std::string typeStr{};
	if (type == Type::VERTEX)
		typeStr = "ERROR::SHADER::VERTEX::COMPILATION_FAILED: \n";
	else if (type == Type::FRAGMENT)
		typeStr = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: \n";
	else if (type == Type::GEOMETRY)
		typeStr = "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED: \n";
	else if (type == Type::PROGRAM)
		typeStr = "ERROR::PROGRAM::LINKING_FAILED: \n";

	int success;
	char infoLog[512];
	if (type == Type::PROGRAM)
	{
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << typeStr << infoLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << typeStr << infoLog << std::endl;
		};
	}
	return static_cast<bool>(success);
}
