#include "engine_pch.h"
#include "resources/OpenGLTextures.h"
#include "core/application.h"

namespace Engine
{
	unsigned int OpenGLTexture::s_slot = 0;

	OpenGLTexture::OpenGLTexture()
	{
	}

	OpenGLTexture::OpenGLTexture(const std::string path)
	{	
		m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
	}

	OpenGLTexture::OpenGLTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char * texData)
	{
		glGenTextures(1, &m_texID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_slot);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (texData)
		{
			if (channels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
			else if (channels == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
			else return;
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			return;
		}
	}

	unsigned int OpenGLTexture::getWidth() const
	{
		return m_width;
	}

	unsigned int OpenGLTexture::getHeight() const
	{
		return m_height;
	}

	unsigned int OpenGLTexture::getChannels() const
	{
		return m_channels;
	}

	unsigned int OpenGLTexture::getSlot() const
	{
		return m_slot;
	}

	void OpenGLTexture::compile() 
	{
		glGenTextures(1, &m_texID);
		m_slot = s_slot;
		s_slot++;
		glActiveTexture(GL_TEXTURE0 + m_slot);
		glBindTexture(GL_TEXTURE_2D, m_texID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (m_data)
		{
			if (m_channels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
			else if (m_channels == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
			else return;
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			return;
		}

		m_width = m_width;
		m_height = m_height;
		m_channels = m_channels;

		stbi_image_free(m_data);
	}
}

