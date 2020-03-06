#include "engine_pch.h"
#include "rendering/renderer/OpenGLPPRenderer.h"

#include <glad/glad.h>

namespace Engine
{
	OpenGLPPRenderer::OpenGLPPRenderer(std::shared_ptr<Shader> defaultPPRShader)
	{
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