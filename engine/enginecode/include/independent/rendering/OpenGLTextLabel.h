#pragma once
#include "rendering/TextLabel.h"
#include "resources/ResourceManager.h"

namespace Engine
{
	class OpenGLTextLabel : public TextLabel
	{
	private:
		std::string m_font;
		unsigned int m_characterSize;
		std::string m_labelText;
		glm::vec2 m_labelPos;
		float m_orientation;
		float m_scale;
		glm::vec3 m_colour;
		glm::mat4 m_model;

		std::shared_ptr<VertexArray> m_VAO;
		std::shared_ptr<VertexBuffer> m_VBO;
		std::shared_ptr<IndexBuffer> m_IBO;
		std::shared_ptr<Shader> m_textShader;
		std::shared_ptr<Material> m_material;

	public:
		bool setFont(const std::string& fontName) override { m_font = fontName; return true; }
		bool setCharSize(unsigned int charSize) override { m_characterSize = charSize; return true; }
		void setText(const std::string& text) override { m_labelText = text; }
		void setPosition(const glm::vec2& position) override { m_labelPos = position; }
		void setOrientation(float angle) override { m_orientation = angle; }
		void setScale(float scale) override { m_scale = scale; }
		void setColour(const glm::vec3& colour) override { m_colour = colour; }
		const std::string& getFont() const override { return m_font; }
		unsigned int getCharSize() const override { return m_characterSize; }
		const std::string& getText() const override { return m_labelText; }
		glm::vec2 getPosition() const override { return m_labelPos; }
		float getOrientation() const override { return m_orientation; }
		float getScale() const override { return m_scale; }
		glm::vec3 getColour() const override { return m_colour; }
		glm::mat4 getModel() const override { return m_model; }
		std::shared_ptr<Material> getMaterial() const override { return m_material; }

		OpenGLTextLabel(const std::string& fontName, unsigned int charSize, const std::string& text, const glm::vec2& position, float orientation, float scale, const glm::vec3& colour);
	};
}
