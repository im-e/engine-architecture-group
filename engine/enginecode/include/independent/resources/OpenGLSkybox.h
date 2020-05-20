#pragma once

/*! \file OpenGLSkybox.h
\brief OpenGL implementation of a skybox. Currently not working.
*/

#include "resources/Skybox.h"
#include "resources/Shader.h"
#include "stb_image.h"

namespace Engine
{
	/*! \class OpenGLSkybox
	\brief OpenGL implementation of a skybox.

		Currently not working.
	*/
	class OpenGLSkybox : public Skybox
	{
	public:
		//! Creates a skybox \param defSkyboxShader default skybox shader
		OpenGLSkybox(const std::shared_ptr<Shader> defSkyboxShader);
		//! Loads a cube map \param cubeFaces textures
		unsigned int loadCubemap(std::vector<std::string> cubeFaces);
		//! Loads a skybox texture \param path texture path
		unsigned int loadTexture(char const* path);
		//! Draws a skybox \param texID texture to be bound
		void draw(unsigned int texID);
		//! Free resources
		void stop();

		//! Skybox textures
		std::vector<std::string> faces
		{
			"Assets/Textures/Skybox/Space_Right.png",
			"Assets/Textures/Skybox/Space_left.png",
			"Assets/Textures/Skybox/Space_top.png",
			"Assets/Textures/Skybox/Space_bot.png",
			"Assets/Textures/Skybox/Space_front.png",
			"Assets/Textures/Skybox/Space_back.png"
		};
		unsigned int texID; //!< Skybox texture ID
		std::shared_ptr<Shader> m_skyboxShader; //!< Skybox shader
		std::shared_ptr<Shader> m_cubemapShader; //!< Cubemap shader
		std::shared_ptr<VertexArray> m_skyboxVAO; //!< Skybox VAO
		std::shared_ptr<VertexArray> m_cubemapVAO; //!< Cubemap VAO
	};
}