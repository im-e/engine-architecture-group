#pragma once

/*! \file ShaderData.h
\brief Converts various shader data to other data, e.g. bytes
*/
#include <glm/glm.hpp>
#include <string>

namespace Engine
{
	/*! \enum ShaderDataType
		\brief Set of all possible shader data types
	*/
	enum class ShaderDataType
	{
		None = 0, 
		Int, Int2, Int3, Int4, 
		Float, Float2, Float3, Float4, 
		Mat2, Mat3, Mat4, 
		Bool, Sampler2D, SamplerCube
	};

	/*! \class ShaderData
		\brief Converts shader data type to other data, e.g. bytes
	*/
	class ShaderData
	{
	public:
		static unsigned int shaderDataTypeSize(ShaderDataType type); //!< Converts data type to size in bytes
		static ShaderDataType stringToDataType(const std::string& src); //!< Converts string (e.g. "bool") to ShaderDataType
		static unsigned int shaderDataTypeComponentCount(ShaderDataType type); //!< Returns number of components in a shader data type (e.g. 3 for Float3)
		static unsigned int shaderDataTypeToOpenGLType(ShaderDataType type); //!< Converts shader data type to OpenGL type (e.g. Int to GL_INT)
	};
}