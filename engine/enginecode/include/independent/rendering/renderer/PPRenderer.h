#pragma once

/*! \file PPRenderer.h
\brief API agnostic implementation of the engine renderer
*/

#include "rendering/Material.h"
#include "rendering\renderer\Renderer.h"

namespace Engine
{	
	/*! \class PPRenderer
	\brief API agnostic implementation of the post processing renderer
	*/
	class PPRenderer
	{
	public:
		virtual void actionCommand(RenderCommand* command) = 0; //!< Issues commands
		virtual void beginScene(const SceneData& sceneData) = 0; //!< Uploads common data in the scene (frame)
		virtual void endScene() = 0; //!< Ends scene (frame)
		virtual void submit(const std::shared_ptr<Material>& material) = 0; //!< Submits material for drawing \param material material to be submitted
		virtual void flush() = 0; //!< Draw everything (used with batch rendering)
		virtual void setPPShader(std::shared_ptr<Shader> shader) = 0;
		//virtual const int& getColourTextureUnit() = 0;
		//virtual const int& getDepthTextureUnit() = 0;
		virtual void setColourTextureUnit(unsigned int unit) = 0;
		virtual void setDepthTextureUnit(unsigned int unit) = 0;
		//! Creates 3D view for the engine
		static PPRenderer* createPPRenderer(std::shared_ptr<Shader> defaultPPRShader);
	}; 
}