#include "engine_pch.h"
#include "resources/Texture.h"

#include "rendering/RenderAPI.h"
#include "resources/OpenGLTextures.h"

namespace Engine
{
	Texture * Texture::createFromFile(const std::string & path)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLTexture(path);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		}
		return nullptr;
	}

	Texture * Texture::createFromRawData(unsigned int width, unsigned int height, unsigned int channels, unsigned char * texData)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLTexture(width, height, channels, texData);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		}
		return nullptr;
	}

	//Texture * Texture::createSkybox(std::vector<std::string> faces)
	//{
	//	switch (RenderAPI::getAPI())
	//	{
	//	case RenderAPI::API::None:
	//		LogError("Lack of graphics API not supported!");
	//		break;
	//	case RenderAPI::API::OpenGL:
	//		return new OpenGLTexture(faces);
	//		break;
	//	case RenderAPI::API::Direct3D:
	//		LogError("Direct3D not yet supported!");
	//		break;
	//	}
	//	return nullptr;
	//}

}