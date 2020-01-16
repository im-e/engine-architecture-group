#include "engine_pch.h"
#include "rendering/Material.h"

#include "rendering/OpenGLMaterial.h"
#include "rendering/RenderAPI.h"

namespace Engine
{
	Material * Material::create(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLMaterial(shader, VAO);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		}
		LogError("Unspecified render API");
		return nullptr;
	}

	Material * Material::create(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexBuffer>& VBO)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			LogWarn("Use VAO instead of VBO if using OpenGL render API!");
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		}
		LogError("Unspecified render API");
		return nullptr;
	}
}