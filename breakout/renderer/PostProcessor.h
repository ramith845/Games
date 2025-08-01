#pragma once
#include "Shader.h"
#include "Texture.h"

class PostProcessor
{
public:
	PostProcessor(unsigned int width, unsigned int height);
	void BeginRender();
	void EndRender();
	void RenderScreen(float time);
private:
	void initRenderData();
public:
	Shader* m_PostProcessingShader;
	bool m_Confuse, m_Chaos, m_Shake;
private:
	int m_Width, m_Height;
	unsigned int m_RenderFBO;
	unsigned int m_RenderColorRBO;
	unsigned int m_RenderDepthRBO;
	unsigned int m_FBO; 
	Texture2D* m_ScreenTexture;// Texture for the screen quad and m_FBO
	unsigned int m_ScreenVAO, m_ScreenVBO;
};