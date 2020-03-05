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
		std::string m_labelText; //!< Labels text
		/*glm::vec2 m_labelPos; 
		float m_orientation;
		float m_scale;
		glm::vec3 m_colour;
		glm::mat4 m_model;*/

		std::shared_ptr<VertexArray> m_VAO; //!< Label VAO
		std::shared_ptr<VertexBuffer> m_VBO; //!< Label VBO
		std::shared_ptr<IndexBuffer> m_IBO; //!< Label IBO
		std::shared_ptr<Shader> m_textShader; //!< Label Shader
		std::shared_ptr<Material> m_material; //!< Label Material

	public:
		bool setFont(const std::string& fontName) override { m_font = fontName; return true; } //!< Set the text label font
		bool setCharSize(unsigned int charSize) override { m_characterSize = charSize; return true; } //!< Set the text label char size
		void setText(const std::string& text) override { m_labelText = text; } //!< Set the text for the label
		/*void setPosition(const glm::vec2& position) override { m_labelPos = position; }
		void setOrientation(float angle) override { m_orientation = angle; }
		void setScale(float scale) override { m_scale = scale; }
		void setColour(const glm::vec3& colour) override { m_colour = colour; }*/
		const std::string& getFont() const override { return m_font; } //!< Get the label font
		unsigned int getCharSize() const override { return m_characterSize; } //!< Get the char size of the label
		const std::string& getText() const override { return m_labelText; } //!< Get the text of the label
		/*glm::vec2 getPosition() const override { return m_labelPos; }
		float getOrientation() const override { return m_orientation; }
		float getScale() const override { return m_scale; }
		glm::vec3 getColour() const override { return m_colour; }
		glm::mat4 getModel() const override { return m_model; }*/
		std::shared_ptr<Material> getMaterial() const override { return m_material; } //!< Get the text label material

		OpenGLTextLabel(const std::string& key, const std::string& fontName, unsigned int charSize, const std::string& text, const glm::vec2& position, float orientation, float scale, const glm::vec3& colour);
	};
}
