#include "engine_pch.h"
#include "rendering/OpenGLTextLabel.h"

namespace Engine
{
	OpenGLTextLabel::OpenGLTextLabel(const std::string& key, const std::string& fontName, unsigned int charSize, const std::string& text, const glm::vec2& position, float orientation, float scale, const glm::vec3& colour)
	{
		setFont(fontName);
		setCharSize(charSize);
		setText(text);
		setPosition(position);
		setOrientation(orientation);
		setScale(scale);
		setColour(colour);

		m_textShader = ResourceManagerInstance->getShader().getAsset("TextShaderFT");
		m_VAO = ResourceManagerInstance->addVAO(key + "VAO");

		float textVerts[4 * 4];
		unsigned int textIndices[4] = { 0, 1, 2, 3 };

		m_VAO->setIndexBuffer(ResourceManagerInstance->addEBO(key + "IBO", textIndices, 4 * text.length()));
		m_VBO = ResourceManagerInstance->addVBO(key + "VBO", nullptr, sizeof(textVerts) * text.length(), m_textShader->getBufferLayout());

		unsigned int loopCount = 0;
		float xAdvance = 0;

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
		    std::shared_ptr<Character> ch = ResourceManagerInstance->getCharacter(fontName, (*c));

			float width = ch->getSize().x;
			float height = ch->getSize().y;

			glm::vec2 startPos = glm::vec2(ch->getStartUV().x, ch->getStartUV().y);
			glm::vec2 endPos = glm::vec2(ch->getEndUV().x, ch->getEndUV().y);

			glm::vec2 bearing = ch->getBearing();

			float textVertices[4 * 4] = 
			{
				xAdvance, 0, startPos.x, startPos.y,
				width, 0, endPos.x, startPos.y,
				width, height, endPos.x, endPos.y,
				xAdvance, height, startPos.x, endPos.y
			};

			/*float textVertices[4 * 4] =
			{
				0, 0, startPos.x, startPos.y,
				width, 0, endPos.x, startPos.y,
				width, height, endPos.x, endPos.y,
				0, height, startPos.x, endPos.y
			};*/

			//if (loopCount == 0)
			//{
			//	m_VBO = ResourceManagerInstance->addVBO(key + "VBO", textVertices, sizeof(textVertices), m_textShader->getBufferLayout());
			//}

			//else
			//{
			//	m_VBO->edit(textVertices, sizeof(textVertices), 0);
			//	//m_VBO->edit(textVertices, sizeof(textVertices), sizeof(textVertices) * loopCount);
			//}

			m_VBO->edit(textVertices, sizeof(textVertices), sizeof(textVertices) * loopCount);

			loopCount++;
			xAdvance += (ch->getAdvance() >> 6); // Bitshift by 6 to get pixel values
		}
		
		m_VAO->setVertexBuffer(m_VBO);
		m_material = ResourceManagerInstance->addMaterial(key + "Mat", m_textShader, m_VAO);

		//https://learnopengl.com/In-Practice/Text-Rendering
	}
}

