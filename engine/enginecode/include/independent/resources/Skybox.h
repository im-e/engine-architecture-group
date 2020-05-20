#pragma once

/*! \file Skybox.h
\brief Creates a skybox. Currently not working.
*/

#include <vector>
#include "resources/Shader.h"

namespace Engine
{
	/*! \class Skybox
	\brief Creates a skybox.
	
		Currently not working
	*/
	class Skybox
	{
	protected:
		std::vector<std::string> m_paths; //!< Paths to skybox's textures
		unsigned int vao, vbo; //!< Buffers IDs
		unsigned int cubeVAO; //!< Skybox VAO
		unsigned int cubeVBO; //!< Skybox VBO
	public:
		//! Default constructor
		Skybox();
		//! Creates a skybox \param defSkyboxShader default skybox shader
		static Skybox* createSkybox(std::shared_ptr<Shader> defSkyboxShader);

		void createSkyboxCube(); //!< Creates skybox cube
		inline std::vector<std::string> getPaths() { return m_paths; } //! Gets all texture paths \return texture paths
		void draw(unsigned int texID); //! Draws a skybox \param texID texture to be bound
		void stop(); //! Free resources

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

		//! Destructor
		~Skybox();
	};
}
