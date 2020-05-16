#include "engine_pch.h"
#include "rendering/renderer/OpenGLPPRenderer.h"

#include <glad/glad.h>
#include "core/application.h"
#include "resources/OpenGLTextures.h"

namespace Engine
{
	OpenGLPPRenderer::OpenGLPPRenderer(const std::shared_ptr<Shader>& defaultPPRShader)
	{
		m_shader = defaultPPRShader;
		glGenFramebuffers(1, &m_frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);

		glGenTextures(1, &m_colourTexture);
		OpenGLTexture::s_slot++;
		m_colourTextureUnit = OpenGLTexture::s_slot;
		glActiveTexture(GL_TEXTURE0 + m_colourTextureUnit);
		glBindTexture(GL_TEXTURE_2D, m_colourTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Application::getInstance().getWindow()->getWidth(), Application::getInstance().getWindow()->getHeight(), 0, GL_RGB, GL_UNSIGNED_INT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		glGenTextures(1, &m_depthTexture);
		OpenGLTexture::s_slot++;
		m_depthTextureUnit = OpenGLTexture::s_slot;
		glActiveTexture(GL_TEXTURE0 + m_depthTextureUnit);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Application::getInstance().getWindow()->getWidth(), Application::getInstance().getWindow()->getHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourTextureUnit, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTextureUnit, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LogError("ERROR::FRAMGEBUFFER:: Framebuffer is not complete!");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		float vertices[] =
		{
			-1.0, 1.0, 0.0, 0.0, 1.0,
			-1.0, -1.0, 0.0, 0.0, 0.0,
			1.0, -1.0, 0.0, 1.0, 0.0,
			1.0, 1.0, 0.0, 1.0, 1.0
		};

		unsigned int indices[] =
		{
			0, 1, 2, 0, 2, 3
		};

		m_screenQuadVAO = ResourceManagerInstance->addVAO("PPVAO");
		ResourceManagerInstance->addVBO("PPVBO", vertices, sizeof(vertices), m_shader->getBufferLayout());
		ResourceManagerInstance->addEBO("PPVIBO", indices, sizeof(unsigned int) * 6);
		ResourceManagerInstance->getVAO().getAsset("PPVAO")->setVertexBuffer(ResourceManagerInstance->getVBO().getAsset("PPVBO"));
		ResourceManagerInstance->getVAO().getAsset("PPVAO")->setIndexBuffer(ResourceManagerInstance->getEBO().getAsset("PPVIBO"));
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
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
		//glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearColor(0, 0, 1, 1);

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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(m_colourTextureUnit);
		glBindTexture(GL_TEXTURE_2D, m_colourTextureUnit);
		int i = 0;
		m_shader->bind();
		m_shader->uploadData("u_colourTexture", (void*)m_colourTextureUnit);
		m_shader->uploadData("u_effectIndex", (void*)m_effectIndex);
		auto x = ResourceManagerInstance->getVAO().getAsset("PPVAO");
		x->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glEnable(GL_DEPTH_TEST);
	}
}