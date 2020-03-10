#include "engine_pch.h"
#include "rendering/renderer/OpenGLPPRenderer.h"

#include <glad/glad.h>

namespace Engine
{
	OpenGLPPRenderer::OpenGLPPRenderer(std::shared_ptr<Shader> defaultPPRShader)
	{
		glGenFramebuffers(1, &m_frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);

		glGenTextures(1, &m_colourTexture);
		glBindTexture(GL_TEXTURE_2D, m_colourTexture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_windowSizeX, m_windowSizeY, 0, GL_RGB, GL_UNSIGNED_INT, NULL);

		glGenTextures(1, &m_depthTexture);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_windowSizeX, m_windowSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LogError("ERROR::FRAMGEBUFFER:: Framebuffer is not complete!");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//current push
	}

	void OpenGLPPRenderer::actionCommand(RenderCommand * command)
	{
		command->action();

		if (command->isAlive() == false)
		{
			delete command;
		}
	}

	void OpenGLPPRenderer::beginScene(const SceneData & sceneData)
	{

		for (auto uniformPair : sceneData)
		{
			unsigned int offset = 0;
			unsigned int size;
			int i = 0;

			UniformLayout layout = uniformPair.first->getLayout();

			for (auto bufferElement : layout)
			{
				uniformPair.first->setData(bufferElement.getOffset(), bufferElement.getSize(), uniformPair.second[i]);
				i++;
			}
		}
	}
	void OpenGLPPRenderer::endScene()
	{
	}

	void OpenGLPPRenderer::submit(const std::shared_ptr<Material>& material)
	{
		auto shader = material->getShader();
		shader->bind();

		auto geometry = static_cast<VertexArray*>(material->getGeometry());
		geometry->bind();

		auto uniforms = material->getData();
		for (auto it = uniforms.begin(); it != uniforms.end(); ++it)
		{
			shader->uploadData(it->first, it->second);
		}

		glDrawElements(GL_TRIANGLES, geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLPPRenderer::flush()
	{
	}
}