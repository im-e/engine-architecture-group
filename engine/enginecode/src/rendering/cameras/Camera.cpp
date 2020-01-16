#include "engine_pch.h"
#include "rendering/cameras/Camera.h"

#include "rendering/RenderAPI.h"
#include "rendering/cameras/Camera2D.h"
#include "rendering/cameras/Camera3D.h"

namespace Engine
{
	Camera * Camera::create2D(float left, float right, float bottom, float top)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new Camera2D(left, right, bottom, top);
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

	Camera * Camera::create3D(float fov, float aspectRatio, float nearClip, float farClip)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new Camera3D(fov, aspectRatio, nearClip, farClip);
			return nullptr;
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