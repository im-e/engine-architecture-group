#pragma once
/*! \file OpenGLSkybox.h
\brief OpenGL implementation of a skybox.
*/

#include "resources/Skybox.h"
#include "resources/Shader.h"
#include "stb_image.h"

namespace Engine
{
	/*! \class OpenGLSkybox
	\brief OpenGL implementation of a skybox.
	*/
	class OpenGLSkybox : public Skybox
	{
	public:
		OpenGLSkybox(const std::shared_ptr<Shader> defSkyboxShader, const std::shared_ptr<Shader> defCubemapShader);
		unsigned int loadCubemap(std::vector<std::string> cubeFaces);
		unsigned int loadTexture(char const* path);
		void draw(unsigned int texID);
		void stop();

		std::vector<std::string> faces
		{
			"Assets/Textures/Skybox/Space_Right.png",
			"Assets/Textures/Skybox/Space_left.png",
			"Assets/Textures/Skybox/Space_top.png",
			"Assets/Textures/Skybox/Space_bot.png",
			"Assets/Textures/Skybox/Space_front.png",
			"Assets/Textures/Skybox/Space_back.png"
		};
		unsigned int texID;
		std::shared_ptr<Shader> m_skyboxShader;
		std::shared_ptr<Shader> m_cubemapShader;
		void* m_data;
	};
}