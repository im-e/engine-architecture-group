#pragma once
/*! \file Character.h
\brief contains implementation of characters and their data
*/

#include <glm/glm.hpp>

namespace Engine
{
	/*! \class Character
	\brief Character implementation for openGL
	*/
	class Character
	{
	private:
		glm::vec2 m_startUV; //!< UV co-ords of the top left of the character
		glm::vec2 m_endUV; //!< UV co-ords of the bottom right of the character glyph
		glm::ivec2 m_size; //!< Size of the character glyph
		glm::ivec2 m_bearing; //!< Bearing of the character - position relative to an origin
		int m_advance; //!< Distance to the next character
	public:
		/*! Custom constructor
		\param size character size
		\param bearing character bearing
		\param advance character advace
		\param startUV top left UV coordinates
		\param endUV bottom right UV coordinates
		*/
		Character(glm::ivec2 size, glm::ivec2 bearing, int advance, glm::vec2 startUV = glm::vec2(0.0f, 0.0f), glm::vec2 endUV = glm::vec2(0.0f, 0.0f)) : m_startUV(startUV), m_endUV(endUV), m_size(size), m_bearing(bearing), m_advance(advance) {};
		void setUVs(glm::vec2 startUV, glm::vec2 endUV) { m_startUV = startUV; m_endUV = endUV; } //!< Set the UV co-ords \param startUV new top left coords \param endUV new bottom right coords
		inline glm::vec2 getStartUV() { return m_startUV; } //!< Get the top left UV coords \return top left coords
		inline glm::vec2 getEndUV() { return m_endUV; }  //!< Get the bottom right UV coords \return bottom right coords
		inline glm::ivec2 getSize() { return m_size; } //!< Get the size \return character size
		inline glm::ivec2 getBearing() { return m_bearing; } //!< Get the bearing \return bearing
		inline int getAdvance() { return m_advance; } //!< Get the advance \return advance
	};


}
