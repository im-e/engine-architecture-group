#pragma once
#include "PPRenderer.h"

namespace Engine
{
	/*! \class OpenGLPPRenderer
	\brief Renderer implementation for OpenGL. Inherits from Renderer
	*/
	class OpenGLPPRenderer : public PPRenderer
	{
	private:
		std::shared_ptr<Shader> m_shader;
		unsigned int m_frameBufferID;
		unsigned int m_colourTexture;
		unsigned int m_depthTexture;
		unsigned int m_colourTextureUnit = 20;
		unsigned int m_depthTextureUnit = 21;
		std::shared_ptr<VertexArray> m_screenQuadVAO;
		//PerDrawData m_ppUniforms;

	public:
		OpenGLPPRenderer(std::shared_ptr<Shader> defaultPPShader); //!< Default constructor

		// Inherited via Renderer
		void actionCommand(RenderCommand * command) override;
		void beginScene(const SceneData & sceneData) override;
		void endScene() override;
		void submit(const std::shared_ptr<Material>& material) override;
		void flush() override;
		void setPPShader(std::shared_ptr<Shader> shader) override { m_shader = shader; }
		//const int& getColourTextureUnit() const override { return m_colourTextureUnit; }
		//const int& getDepthTextureUnit() const override;
		void setColourTextureUnit(unsigned int unit) { m_colourTextureUnit = unit; }
		void setDepthTextureUnit(unsigned int unit) { m_depthTextureUnit = unit; }
	};
}
