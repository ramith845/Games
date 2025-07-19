#pragma once

#include "glad/glad.h"

class Texture2D
{
public:
	Texture2D(
		int width,
		int height,
		unsigned int format = GL_RGB,
		unsigned int internalFormat = GL_RGB,
		unsigned int wrapS = GL_REPEAT,
		unsigned int wrapT = GL_REPEAT,
		unsigned int minFilter = GL_NEAREST,
		unsigned int maxFilter = GL_LINEAR);

	~Texture2D()
	{

	}

	void Generate(int width, int height, const unsigned char* data);
	void TextureMipMap();
	void Bind() const;


private:
	unsigned int m_ID;
	unsigned int m_WrapS{ GL_REPEAT };
	unsigned int m_WrapT{ GL_REPEAT };
	unsigned int m_MinFilter{ GL_NEAREST };
	unsigned int m_MaxFilter{ GL_LINEAR };
	unsigned int m_Format{ GL_RGB };
	unsigned int m_InternalFormat{ GL_RGB };

	unsigned int m_Width{};
	unsigned int m_Height{};
};

