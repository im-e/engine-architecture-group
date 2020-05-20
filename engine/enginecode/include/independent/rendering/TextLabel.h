#pragma once

/*! \file TextLabel.h
\brief Defines a 2D text
*/

#include <string>
#include <glm/glm.hpp>
#include "rendering/Material.h"

namespace Engine
{
	/*! \class TextLabel
	\brief Defines a 2D text
	*/
	class TextLabel
	{
	public:
		//! Sets font \param fontName new font
		virtual bool setFont(const std::string& fontName) = 0;
		//! Sets character size \param charSize new size
		virtual bool setCharSize(unsigned int charSize) = 0;
		//! Sets text \param text new text to be displayed
		virtual void setText(const std::string& text) = 0;
		//! Sets the text label's name \param name new name
		virtual void setName(const std::string& name) = 0;
		//! Get the label font \return current font name
		virtual const std::string& getFont() const = 0;
		//! Get the char size of the label \return current character size
		virtual unsigned int getCharSize() const = 0;
		//! Get the text of the label \return current text
		virtual const std::string& getText() const = 0;
		//! Get the text labels name \return current name
		virtual const std::string& getName() const = 0;
		//! Get the text label material \return current material
		virtual std::shared_ptr<Material> getMaterial() const = 0;

		/*! Creates a text label
		\param key name of the label
		\param fontName name of the font
		\param charSize character size
		\param text to be displayed
		\param position label position
		\param orientation label rotation
		\param scale label scale
		\param colour text color
		*/
		static TextLabel* create(const std::string& key, const std::string& fontName, unsigned int charSize, const std::string& text, const glm::vec2& position, float orientation, float scale, const glm::vec3& colour);
		//! Edits text \param text new text
		virtual void editText(const std::string& text) = 0;
	};

}
