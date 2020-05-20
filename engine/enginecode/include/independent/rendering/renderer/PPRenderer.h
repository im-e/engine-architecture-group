#pragma once

/*! \file PPRenderer.h
\brief API agnostic implementation of the engine renderer
*/

#include "rendering/Material.h"
#include "rendering\renderer\Renderer.h"
#include "windows\window.h"

namespace Engine
{	
	/*! \class PPRenderer
	\brief API agnostic implementation of the post processing renderer
	*/
	class PPRenderer : public Renderer
	{
	public:
		//! Sets postprocessing shader \param shader new postprocessing shader
		virtual void setPPShader(std::shared_ptr<Shader> shader) = 0;
		//! Sets postprocessing effect index \param PPIndex new effect index
		virtual void setPPIndex(int PPIndex) = 0;
		//! Sets colour texture unit \param unit new unit
		virtual void setColourTextureUnit(unsigned int unit) = 0;
		//! Sets depth texture unit \param unit new unit
		virtual void setDepthTextureUnit(unsigned int unit) = 0;
		//! Creates a postprocessing renderer \param defaultPPRShader default postprocessing shader
		static PPRenderer* createPPRenderer(std::shared_ptr<Shader> defaultPPRShader);
	}; 
}