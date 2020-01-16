#include "engine_pch.h"
#include "rendering/RenderAPI.h"

namespace Engine
{
	//currently supporting only OpenGL
	RenderAPI::API RenderAPI::s_api = RenderAPI::API::OpenGL;
}