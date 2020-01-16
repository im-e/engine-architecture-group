#include "engine_pch.h"
#include <glad/glad.h>

#include "rendering/ShaderData.h"

namespace Engine
{
	unsigned int ShaderData::shaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Int:
			return sizeof(int);
		case ShaderDataType::Int2:
			return sizeof(int) * 2;
		case ShaderDataType::Int3:
			return sizeof(int) * 3;
		case ShaderDataType::Int4:
			return sizeof(int) * sizeof(int);

		case ShaderDataType::Float:
			return sizeof(float);
		case ShaderDataType::Float2:
			return sizeof(float) * 2;
		case ShaderDataType::Float3:
			return sizeof(float) * 3;
		case ShaderDataType::Float4:
			return sizeof(float) * sizeof(float);

		case ShaderDataType::Mat2:
			return sizeof(glm::mat2);
		case ShaderDataType::Mat3:
			return sizeof(glm::mat3);
		case ShaderDataType::Mat4:
			return sizeof(glm::mat4);

		case ShaderDataType::Bool:
			return 1;
		case ShaderDataType::Sampler2D:
			return 1;

		}

		return 0;
	}

	ShaderDataType ShaderData::stringToDataType(const std::string & src)
	{
		ShaderDataType result = ShaderDataType::None;

		if (src == "bool")
			result = ShaderDataType::Bool;
		else if (src == "int")
			result = ShaderDataType::Int;
		else if (src == "ivec2")
			result = ShaderDataType::Int2;
		else if (src == "ivec3")
			result = ShaderDataType::Int3;
		else if (src == "ivec4")
			result = ShaderDataType::Int4;
		else if (src == "float")
			result = ShaderDataType::Float;
		else if (src == "vec2")
			result = ShaderDataType::Float2;
		else if (src == "vec3")
			result = ShaderDataType::Float3;
		else if (src == "vec4")
			result = ShaderDataType::Float4;
		else if (src == "mat2")
			result = ShaderDataType::Mat2;
		else if (src == "mat3")
			result = ShaderDataType::Mat3;
		else if (src == "mat4")
			result = ShaderDataType::Mat4;
		else if (src == "sampler2D")
			result = ShaderDataType::Sampler2D;

		return result;
	}

	unsigned int ShaderData::shaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Int:
			return 1;
		case ShaderDataType::Int2:
			return glm::ivec2::length();
		case ShaderDataType::Int3:
			return glm::ivec3::length();
		case ShaderDataType::Int4:
			return glm::ivec4::length();

		case ShaderDataType::Float:
			return 1;
		case ShaderDataType::Float2:
			return glm::vec2::length();
		case ShaderDataType::Float3:
			return glm::vec3::length();
		case ShaderDataType::Float4:
			return glm::vec4::length();

		case ShaderDataType::Mat2:
			return glm::mat2::length();
		case ShaderDataType::Mat3:
			return glm::mat3::length();
		case ShaderDataType::Mat4:
			return glm::mat4::length();

		case ShaderDataType::Bool:
			return 1;
		case ShaderDataType::Sampler2D:
			return 1;
		}

		return 0;
	}

	unsigned int ShaderData::shaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Int:
			return GL_INT;
		case ShaderDataType::Int2:
			return GL_INT;
		case ShaderDataType::Int3:
			return GL_INT;
		case ShaderDataType::Int4:
			return GL_INT;

		case ShaderDataType::Float:
			return GL_FLOAT;
		case ShaderDataType::Float2:
			return GL_FLOAT;
		case ShaderDataType::Float3:
			return GL_FLOAT;
		case ShaderDataType::Float4:
			return GL_FLOAT;

		case ShaderDataType::Mat2:
			return GL_FLOAT;
		case ShaderDataType::Mat3:
			return GL_FLOAT;
		case ShaderDataType::Mat4:
			return GL_FLOAT;

		case ShaderDataType::Bool:
			return GL_BOOL;
		case ShaderDataType::Sampler2D:
			return GL_SAMPLER_2D;
		}

		return 0;
	}
}