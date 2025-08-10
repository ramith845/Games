#include "pch.h"
#include "Texture.h"

Texture2D::Texture2D(
	int width,
	int height,
	unsigned int format,
	unsigned int internalFormat,
	unsigned int wrapS,
	unsigned int wrapT,
	unsigned int minFilter,
	unsigned int maxFilter)
	: m_ID(0),
	m_Width(static_cast<unsigned int>(width)),
	m_Height(static_cast<unsigned int>(height)),
	m_Format(format),
	m_InternalFormat(internalFormat),
	m_WrapS(wrapS),
	m_WrapT(wrapT),
	m_MinFilter(minFilter),
	m_MaxFilter(maxFilter)
{
	glGenTextures(1, &m_ID);
}

Texture2D::~Texture2D()
{
	//std::println("[Texture2D] Cleaning and deleting resources...");
	//std::println("	- deleting texture: {}", m_ID);
	glDeleteTextures(1, &m_ID);
	//std::println("[Texture2D] Cleaned up.");
}

unsigned int Texture2D::GetID() const
{
	return m_ID;
}

void Texture2D::Generate(int width, int height, const unsigned char* data)
{
	if (m_ID == 0)
		return;
	m_Width = static_cast<unsigned int>(width);
	m_Height = static_cast<unsigned int>(height);

	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(m_InternalFormat), width, height, 0, m_Format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(m_WrapS)); //10487
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(m_WrapT)); //10487
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(m_MinFilter)); //9728
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(m_MaxFilter)); //9728
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::TextureMipMap() const
{
	Bind();
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

