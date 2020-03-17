#pragma once

/*! \file Shader.h
\brief Base implementation of a shader
*/

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "rendering/Buffers.h"

namespace Engine
{
	/*! \class Shader
	\brief API agnostic base class for all shaders
	*/
	class Shader
	{
	protected:
		unsigned int m_ID; //!< Unique ID of a shader
		unsigned int m_vertID; //!< Unique ID of a vertex shader
		unsigned int m_fragID; //!< Unique ID of a fragment shader

		BufferLayout m_bufferLayout; //!< Buffer layout extracted from a shader code
		std::map<std::string, std::pair<ShaderDataType, unsigned int>> m_uniformCache; //!< Cached uniforms from shader code

	public:
		virtual unsigned int getID() = 0; //!< Get unique ID \return shader's ID
		virtual void bind() = 0; //!< Use shader in the application
		virtual void unbind() = 0; //!< Stop using shader in the application
		virtual bool uploadData(const std::string& dataName, void* data) = 0; //!< Feed shader with data \param dataName name of the uniform \param data data to be passed
		virtual BufferLayout& getBufferLayout() = 0; //!< Get extracted buffer layout \return buffer layout
		virtual std::map<std::string, std::pair<ShaderDataType, unsigned int>> getUniformCache() = 0; //!< Get the whole block of uniforms in the shader \return map of uniforms from the shader

		/*! Create a shader based on current RenderAPI 
		\param path path to the shader file
		*/
		static Shader* create(const std::string& path);
		/*! Create a shader based on current RenderAPI 
		\param vertexPath path to the vertex shader file 
		\param fragmentPath path to the fragment shader file
		*/
		static Shader* create(const std::string& vertexPath, const std::string& fragmentPath); 
	};
}