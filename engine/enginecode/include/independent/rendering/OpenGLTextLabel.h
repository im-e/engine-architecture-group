#pragma once
/*! \file OpenGLTextLabel.h
\brief contains implementation of an OpenGLTextLabel
*/

#include "rendering/TextLabel.h"
#include "resources/ResourceManager.h"

namespace Engine
{
	/*! \class OpenGLTextLabel
	\brief Text Label implementation for OpenGL
	*/
	class OpenGLTextLabel : public TextLabel
	{
	private:
		std::string m_font; //!< Font used in the text label
		unsigned int m_characterSize; //!< Character size
		std::string m_labelText; //!< Label's text
		std::string m_name; //!< Label's name

		std::shared_ptr<VertexArray> m_VAO; //!< Label VAO
		std::shared_ptr<VertexBuffer> m_VBO; //!< Label VBO
		std::shared_ptr<IndexBuffer> m_IBO; //!< Label EBO
		std::shared_ptr<Shader> m_textShader; //!< Label Shader
		std::shared_ptr<Material> m_material; //!< Label Material

	public:
		bool setFont(const std::string& fontName) override { m_font = fontName; return true; }
		bool setCharSize(unsigned int charSize) override { m_characterSize = charSize; return true; }
		void setText(const std::string& text) override { m_labelText = text; }
		void setName(const std::string& name) override { m_name = name; } 
		const std::string& getFont() const override { return m_font; }
		unsigned int getCharSize() const override { return m_characterSize; }
		const std::string& getText() const override { return m_labelText; }
		const std::string& getName() const override { return m_name; }
		std::shared_ptr<Material> getMaterial() const override { return m_material; }

		/*! Custom constructor
		\param key name of the label
		\param fontName name of the font
		\param charSize character size
		\param text to be displayed
		\param position label position
		\param orientation label rotation
		\param scale label scale
		\param colour text color
		*/
		OpenGLTextLabel(const std::string& key, const std::string& fontName, unsigned int charSize, const std::string& text, const glm::vec2& position, float orientation, float scale, const glm::vec3& colour);
		void editText(const std::string& text) override;
	};
}
