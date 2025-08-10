#pragma once
#include "Shader.h"
#include "Texture.h"

class PostProcessor
{
public:
	PostProcessor(unsigned int width, unsigned int height);
	~PostProcessor();
	void BeginRender();
	void EndRender();
	void RenderScreen(float time);
private:
	void initRenderData();
public:
	bool m_Confuse{ false }, m_Chaos{ false }, m_Shake{ false };
private:
	ShaderPtr m_PostProcessingShader;
	int m_Width, m_Height;
	unsigned int m_RenderFBO;
	unsigned int m_RenderColorRBO;
	unsigned int m_RenderDepthRBO;
	unsigned int m_FBO;
	// careful to deep copy this
	Texture2D* m_ScreenTexture;// Texture for the screen quad and m_FBO
	unsigned int m_ScreenVAO, m_ScreenVBO;
};