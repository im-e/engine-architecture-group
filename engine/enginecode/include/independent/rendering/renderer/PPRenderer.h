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
		virtual void setPPShader(std::shared_ptr<Shader> shader) = 0;
		virtual void setPPIndex(int PPIndex) = 0;
		//virtual const int& getColourTextureUnit() = 0;
		//virtual const int& getDepthTextureUnit() = 0;
		virtual void setColourTextureUnit(unsigned int unit) = 0;
		virtual void setDepthTextureUnit(unsigned int unit) = 0;
		//! Creates 3D view for the engine
		static PPRenderer* createPPRenderer(std::shared_ptr<Shader> defaultPPRShader);
	}; 
}