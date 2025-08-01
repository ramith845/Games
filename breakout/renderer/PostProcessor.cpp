#include "pch.h"
#include "PostProcessor.h"
#include "manager/ResourceManager.h"
#include "GLFW/glfw3.h"

constexpr const int MSAASamples = 4;

PostProcessor::PostProcessor(unsigned int width, unsigned int height)
	: m_Width((int)width), m_Height((int)height)
{
	// Create a framebuffer object for MSAA rendering
	glGenFramebuffers(1, &m_RenderFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderFBO);

	// Create a renderbuffer object for color and depth/stencil attachments
	glGenRenderbuffers(1, &m_RenderColorRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderColorRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAASamples, GL_RGB, (int)m_Width, (int)m_Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RenderColorRBO);

	glGenRenderbuffers(1, &m_RenderDepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderDepthRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAASamples, GL_DEPTH24_STENCIL8, (int)m_Width, (int)m_Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderDepthRBO);

	bool result1{ glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE };
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Create a second framebuffer for rendering to a texture
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// Create a texture to render to
	m_ScreenTexture = new Texture2D(
		m_Width, m_Height, GL_RGB, GL_RGB, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR
	);
	m_ScreenTexture->Generate(m_Width, m_Height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ScreenTexture->GetID(), 0);

	bool result2{ glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE };
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (!(result1 && result2))
	{
		glfwTerminate();
		throw std::runtime_error("PostProcessor: Failed to create MSAA framebuffer or texture framebuffer");
	}

	m_PostProcessingShader = ResourceManager::LoadShader("shaders/3.3.aa_post.vert", "shaders/3.3.aa_post.frag", "screen");
	m_PostProcessingShader->use()->SetInt("screenTexture", 0);
	float offset = 1 / 300.f;
	float offsets[9][2] = {
		{-offset,  offset}, {0.0f,  offset}, {offset,  offset},
		{-offset,    0.0f}, {0.0f,    0.0f}, {offset,    0.0f},
		{-offset, -offset}, {0.0f, -offset}, {offset, -offset}
	};
	m_PostProcessingShader->SetVec2fv("offset", 9, &offset);

	float blurKernel[9] = {
		0.0625, 0.125, 0.0625,
		 0.125,  0.25,  0.125,
		0.0625, 0.125, 0.0625
	};
	m_PostProcessingShader->Set1fv("blur_kernel", 9, blurKernel);

	initRenderData();
}

void PostProcessor::initRenderData()
{
	float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	// setup screen VAO
	glGenVertexArrays(1, &m_ScreenVAO);
	glGenBuffers(1, &m_ScreenVBO);
	glBindVertexArray(m_ScreenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ScreenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void PostProcessor::EndRender()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RenderFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
	glBlitFramebuffer(0, 0, (int)m_Width, (int)m_Height, 0, 0,
		(int)m_Width, (int)m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderFBO);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessor::RenderScreen(float time)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	m_PostProcessingShader->use()->SetFloat("time", time);
	m_PostProcessingShader->SetBool("shake", m_Shake);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ScreenTexture->GetID());

	glBindVertexArray(m_ScreenVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}