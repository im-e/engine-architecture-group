#include "engine_pch.h"
#include "rendering/renderer/PPRenderer.h"

#include "rendering/RenderAPI.h"
#include "rendering/renderer/OpenGLPPRenderer.h"
#include "rendering/renderer/OpenGLRenderCommands.h"

namespace Engine
{
	PPRenderer * PPRenderer::createPPRenderer(std::shared_ptr<Shader> defaultPPRShader)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLPPRenderer(defaultPPRShader);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		default:
			LogError("Unknown graphics API - please specify");
			return nullptr;
			break;
		}
	}
}