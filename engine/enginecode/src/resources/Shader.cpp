#include "engine_pch.h"
#include "resources/Shader.h"

#include "resources/OpenGLShader.h"
#include "rendering/RenderAPI.h"

namespace Engine
{
	Shader * Shader::create(const std::string & path)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLShader(path);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		}
		return nullptr;
	}

	Shader * Shader::create(const std::string & vertexPath, const std::string & fragmentPath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLShader(vertexPath, fragmentPath);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		}
		return nullptr;
	}
}