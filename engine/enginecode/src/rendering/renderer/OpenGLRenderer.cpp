#include "engine_pch.h"
#include "rendering/renderer/OpenGLRenderer.h"

#include <glad/glad.h>
#include "resources/ResourceManager.h"

namespace Engine
{

	/**********************************
				3D Renderer
	**********************************/
	OpenGLRenderer::OpenGLRenderer()
	{
	}

	void OpenGLRenderer::actionCommand(RenderCommand * command)
	{
		command->action();

		if (command->isAlive() == false)
		{
			delete command;
		}		
	}

	void OpenGLRenderer::beginScene(const SceneData & sceneData)
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

	void OpenGLRenderer::endScene()
	{
	}

	void OpenGLRenderer::submit(const std::shared_ptr<Material>& material)
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

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode.
		glDrawElements(GL_PATCHES, geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr); // Render the scene.
	}

	void OpenGLRenderer::flush()
	{
	}

	/**********************************
				2D Renderer
	**********************************/
	OpenGL2DRenderer::OpenGL2DRenderer()
	{
	}

	void OpenGL2DRenderer::actionCommand(RenderCommand * command)
	{
		command->action();

		if (command->isAlive() == false)
		{
			delete command;
		}	
	}

	void OpenGL2DRenderer::beginScene(const SceneData & sceneData)
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

	void OpenGL2DRenderer::endScene()
	{
	}

	void OpenGL2DRenderer::submit(const std::shared_ptr<Material>& material)
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

		glActiveTexture(ResourceManagerInstance->getFontTexture()->getSlot()+1);
		glBindTexture(GL_TEXTURE_2D, ResourceManagerInstance->getFontTexture()->getSlot()+1);
		glDrawArrays(GL_QUADS, 0, geometry->getDrawCount());
	}

	void OpenGL2DRenderer::flush()
	{
		
	}
}