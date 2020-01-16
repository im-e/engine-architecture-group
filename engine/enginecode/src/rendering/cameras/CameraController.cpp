#include "engine_pch.h"
#include "rendering/cameras/CameraController.h"

#include "rendering/RenderAPI.h"
#include "rendering/cameras/Camera2DController.h"
#include "rendering/cameras/Camera3DController.h"

namespace Engine
{
	CameraController * CameraController::create2DController(float translationSpeed)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new Camera2DController(translationSpeed);
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

	CameraController * CameraController::create3DController(float translationSpeed, float rotationSpeed)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new Camera3DController(translationSpeed, rotationSpeed);
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