#include "engine_pch.h"
#include "rendering/TextLabel.h"
#include "rendering/OpenGLTextLabel.h"
#include "rendering/RenderAPI.h"

namespace Engine
{
	TextLabel * TextLabel::create(const std::string& fontName, unsigned int charSize, const std::string& text, const glm::vec2& position, float orientation, float scale, const glm::vec3& colour)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLTextLabel(fontName, charSize, text, position, orientation, scale, colour);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		}
		LogError("Unspecified render API");
		return nullptr;
	}
}