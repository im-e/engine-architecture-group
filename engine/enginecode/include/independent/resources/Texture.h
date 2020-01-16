#pragma once

/*! \file Texture.h
\brief Base implementation of a texture
*/

#include <string>
#include <vector>
#include <stb_image.h>

namespace Engine
{
	/*! \class Texture
	\brief Base class for all other texture types (API agnostic)
	*/
	class Texture
	{
	protected:
		unsigned int m_texID; //!< Unique ID
		unsigned int m_slot; //!< Slot in the memory where a texture is stored
		
		unsigned int m_width; //!< Width of a texture
		unsigned int m_height; //!< Height of a texture 
		unsigned int m_channels; //!< How many channels does a texture have (RGBA format)

	public:
		virtual unsigned int getWidth() const = 0; //!< Get width \return width of the texture
		virtual unsigned int getHeight() const = 0; //!< Get height \return height of the texture
		virtual unsigned int getChannels() const = 0; //!< Get channels \return number of channels of the texture
		virtual unsigned int getSlot() const = 0; //!< Get slot \return texture storage in a memory

		/*!Creates a texture based on current RenderAPI 
		\param path path to the texture on the disc
		\return new RenderAPI specific texture
		*/
		static Texture* createFromFile(const std::string& path);
		/*!  Creates a texture based on current RenderAPI 
		\param width width of the texture 
		\param height height of the texture 
		\param channels channels of the texture 
		\param texData pointer to texture data in the memory
		\return new RenderAPI specific texture
		*/
		static Texture* createFromRawData(unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData);
	};
}
