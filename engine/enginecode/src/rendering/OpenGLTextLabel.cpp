#include "engine_pch.h"
#include "rendering/OpenGLTextLabel.h"

namespace Engine
{
	OpenGLTextLabel::OpenGLTextLabel(const std::string& fontName, unsigned int charSize, const std::string& text, const glm::vec2& position, float orientation, float scale, const glm::vec3& colour)
	{
		setFont(fontName);
		setCharSize(charSize);
		setText(text);
		setPosition(position);
		setOrientation(orientation);
		setScale(scale);
		setColour(colour);

		m_textShader = ResourceManagerInstance->addShader("textShaderFT", "assets/shaders/textShaderFT.glsl");
		m_VAO = ResourceManagerInstance->addVAO(text);

		unsigned int textIndices[4] = { 0, 1, 2, 3 };

		m_VAO->setIndexBuffer(ResourceManagerInstance->addEBO(text, textIndices, 4));

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			std::shared_ptr<Character> ch = ResourceManagerInstance->getCharacter(fontName, *c);

			glm::vec2 startPos = glm::vec2(ch->getStartUV().x, ch->getStartUV().y) * scale;
			glm::vec2 endPos = glm::vec2(ch->getEndUV().x, ch->getEndUV().y) * scale;

			float width = ch->getSize().x * scale;
			float height = ch->getSize().y * scale;

			float textVertices[4 * 4] = {
				0, 0, startPos.x, startPos.y,
				width, 0, endPos.x, startPos.y,
				width, height, endPos.x, endPos.y,
				0, height, startPos.x, endPos.y
			};

			//m_VAO->setVertexBuffer(ResourceManagerInstance->addVBO(text, textVertices, sizeof(textVertices), m_textShader->getBufferLayout()));
		}
		
		m_material = ResourceManagerInstance->addMaterial(text, m_textShader, m_VAO);

		//https://learnopengl.com/In-Practice/Text-Rendering
	}
}