#include "engine_pch.h"
#include "rendering/OpenGLTextLabel.h"

/*! \file OpenGLTextLabel.cpp
\brief Contains source code for creating an OpenGL text label
*/

namespace Engine
{
	OpenGLTextLabel::OpenGLTextLabel(const std::string& key, const std::string& fontName, unsigned int charSize, const std::string& text, const glm::vec2& position, float orientation, float scale, const glm::vec3& colour)
	{
		setFont(fontName);
		setCharSize(charSize);
		setName(key);

		m_textShader = ResourceManagerInstance->getShader().getAsset("TextShaderFT");
		m_VAO = ResourceManagerInstance->addVAO(key + "VAO");

		editText(text);

		//https://learnopengl.com/In-Practice/Text-Rendering
	}

	void OpenGLTextLabel::editText(const std::string& text)
	{
		setText(text);

		float textVerts[4 * 4];
		unsigned int textIndices[4] = { 0, 1, 2, 3 };

		m_VAO->setIndexBuffer(ResourceManagerInstance->overwriteEBO(m_name + "EBO", textIndices, 4 * text.length()));
		m_VBO = ResourceManagerInstance->overwriteVBO(m_name + "VBO", nullptr, sizeof(textVerts) * text.length(), m_textShader->getBufferLayout());
			
		unsigned int loopCount = 0;
		float xAdvance = 0;

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			std::shared_ptr<Character> ch = ResourceManagerInstance->getCharacter(getFont(), (*c));

			glm::vec2 bearing = ch->getBearing();

			float width = ch->getSize().x;
			float height = ch->getSize().y;

			glm::vec2 startPos = glm::vec2(ch->getStartUV().x, ch->getStartUV().y);
			glm::vec2 endPos = glm::vec2(ch->getEndUV().x, ch->getEndUV().y);

			float xPos = xAdvance + bearing.x;
			float yPos = 0 - (height - bearing.y);

			float textVertices[4 * 4] =
			{
				xPos,						-yPos - height,					startPos.x,		startPos.y,				// Top Left
				xPos + width,				-yPos - height,					endPos.x,		startPos.y,				// Top Right
				xPos + width,				-yPos,							endPos.x,		endPos.y,				// Bottom Right
				xPos,						-yPos,							startPos.x,		endPos.y				// Bottom Left
			};

			m_VBO->edit(textVertices, sizeof(textVertices), sizeof(textVertices) * loopCount);

			loopCount++;
			xAdvance += (ch->getAdvance() >> 6); // Bitshift by 6 to get pixel values
		}

		m_VAO->setVertexBuffer(m_VBO);

		m_material = ResourceManagerInstance->overwriteMaterial(m_name + "Mat", m_textShader, m_VAO);
	}
}

