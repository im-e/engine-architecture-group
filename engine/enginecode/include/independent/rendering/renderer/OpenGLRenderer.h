#pragma once

/*! \file OpenGLRenderer.h
\brief OpenGL implementation for renderer
*/

#include "Renderer.h"

namespace Engine
{
	/*! \class OpenGLRenderer
	\brief Renderer implementation for OpenGL. Inherits from Renderer 
	*/
	class OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer(); //!< Default constructor

		// Inherited via Renderer
		void actionCommand(RenderCommand * command) override;
		void beginScene(const SceneData & sceneData) override;
		void endScene() override;
		void submit(const std::shared_ptr<Material>& material) override;
		void tessSubmit(const std::shared_ptr<Material>& material) override;
		void flush() override;
	};

	/*! \class OpenGL2DRenderer
	\brief 2D renderer implementation for OpenGL. Inherits from Renderer
	*/
	class OpenGL2DRenderer : public Renderer
	{
	public:
		OpenGL2DRenderer(); //!< Default constructor

		// Inherited via Renderer
		void actionCommand(RenderCommand * command) override;
		void beginScene(const SceneData & sceneData) override;
		void endScene() override;
		void submit(const std::shared_ptr<Material>& material) override;
		void tessSubmit(const std::shared_ptr<Material>& material) override;
		void flush() override;
	};
}