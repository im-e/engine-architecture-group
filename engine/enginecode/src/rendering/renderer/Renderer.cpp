#include "engine_pch.h"
#include "rendering/renderer/Renderer.h"

#include "rendering/RenderAPI.h"
#include "rendering/renderer/OpenGLRenderer.h"
#include "rendering/renderer/OpenGLRenderCommands.h"

namespace Engine
{
	Renderer * Renderer::create2D()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGL2DRenderer();
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

	Renderer * Renderer::create3D()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLRenderer();
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

	RenderCommand * RenderCommand::clearDepthColorBuffer(bool alive)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLClearDepthColorBufferCommand(alive);
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

	RenderCommand * RenderCommand::setClearColor(bool alive, float r, float g, float b, float a)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLSetClearColorCommand(alive, r, g, b, a);
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

	RenderCommand * RenderCommand::setDepthTestLest(bool alive, bool enabled)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLSetDepthTestLessCommand(alive, enabled);
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

	RenderCommand * RenderCommand::setBackfaceCulling(bool alive, bool enabled)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLSetBackfaceCullingCommand(alive, enabled);
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

	RenderCommand * RenderCommand::setDebugOutput(bool alive, bool enabled)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLSetDebugCommand(alive, enabled);
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

	RenderCommand * RenderCommand::setMinusOneAlphaBlending(bool alive, bool enabled)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLSetMinusOneAlphaBlending(alive, enabled);
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