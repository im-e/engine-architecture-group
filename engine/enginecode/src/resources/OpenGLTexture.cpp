#include "engine_pch.h"
#include "resources/OpenGLTextures.h"

namespace Engine
{
	OpenGLTexture::OpenGLTexture()
	{
	}

	OpenGLTexture::OpenGLTexture(const std::string & path)
	{
		glGenTextures(1, &m_texID);
		m_slot = m_texID;
		glActiveTexture(GL_TEXTURE0 + m_slot);
		glBindTexture(GL_TEXTURE_2D, m_slot);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, channels;

		unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		m_width = width;
		m_height = height;
		m_channels = channels;
		
		if (data)
		{
			if (channels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (channels == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else return;
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			return;
		}
		stbi_image_free(data);
	}

	OpenGLTexture::OpenGLTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char * texData)
	{
		m_width = width;
		m_height = height;
		m_channels = channels;

		glGenTextures(1, &m_texID);
		m_slot = m_texID;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_slot);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 3)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
		else if (channels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		}
		else if (channels == 1)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, texData);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		}
		else return;
		glGenerateMipmap(GL_TEXTURE_2D);

		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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
		}*/
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
}

