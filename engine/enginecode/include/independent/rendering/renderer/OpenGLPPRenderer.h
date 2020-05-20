#pragma once
/*! \file OpenGLPPRenderer.h
\brief Defines an OpenGL postprocessing renderer
*/

#include "PPRenderer.h"
#include "rendering/Buffers.h"
#include "resources/ResourceManager.h"

namespace Engine
{
	/*! \class OpenGLPPRenderer
	\brief Renderer implementation for OpenGL. Inherits from Renderer
	*/
	class OpenGLPPRenderer : public PPRenderer
	{
	private:
		std::shared_ptr<Shader> m_shader; //!< Pointer to a shder
		unsigned int m_frameBufferID; //!< Frame buffer ID
		unsigned int m_colourTexture; //!< Colour texture ID
		unsigned int m_depthTexture; //!< Depth texture ID
		unsigned int m_colourTextureUnit = 14; //!< Colour texture unit ID
		unsigned int m_depthTextureUnit = 15; //!< Depth texture unit ID
		unsigned int m_effectIndex = 2; //!< Postprocessing effect index
		std::shared_ptr<VertexArray> m_screenQuadVAO; //!< Screen texture

	public:
		//! Custom constructor \param defaultPPShader default postprocessing shader
		OpenGLPPRenderer(const std::shared_ptr<Shader>& defaultPPShader); 

		// Inherited via Renderer
		void actionCommand(RenderCommand * command) override;
		void beginScene(const SceneData & sceneData) override;
		void endScene() override;
		void submit(const std::shared_ptr<Material>& material) override;
		void flush() override;
		void setPPShader(std::shared_ptr<Shader> shader) override { m_shader = shader; }
		void setPPIndex(int PPIndex) override { m_effectIndex = PPIndex; }
		void setColourTextureUnit(unsigned int unit) { m_colourTextureUnit = unit; }
		void setDepthTextureUnit(unsigned int unit) { m_depthTextureUnit = unit; }
		std::shared_ptr<VertexArray> returnVAO() { return m_screenQuadVAO; }
	};
}
