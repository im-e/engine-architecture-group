#pragma once

/*! \file OpenGLTextures.h
\brief Implementation of OpenGL textures
*/
#include "Texture.h"
#include <glad/glad.h>

namespace Engine
{
	/*! \class OpenGLTexture
	\brief API specific texture implementation (OpenGL)
	*/
	class OpenGLTexture : public Texture
	{
	private:
		static unsigned int s_slot; //!< Static slot in the memory where a texture is stored

		unsigned char * m_data; //! Texture data
		int m_width; //! Texture width
		int m_height; //! Texture height
		int m_channels; //! Texture channels
	public:
		OpenGLTexture(); //!< Default constructor 
		OpenGLTexture(const std::string path); //!< Custom constructor \param path path to the texture file
		/*! Custom constructor 
		\param width width of the texture 
		\param height height of the texture 
		\param channels number of texture channels 
		\param texData pointer to texture data in the memory
		*/
		OpenGLTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData); 

		// Inherited via Texture
		unsigned int getWidth() const override; //!< Get width of the texture \return width
		unsigned int getHeight() const override; //!< Get height of the texture \return height
		unsigned int getChannels() const override; //!< Get channels of the texture \return channels
		unsigned int getSlot() const override; //!< Get slot where the texture is stored \return slot

		void compile() override;
	};
}