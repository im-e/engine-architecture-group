#pragma once

/*! \file OpenGLShader.h
\brief OpenGL specific shader implementation
*/

#include <functional>

#include "Shader.h"
#include "rendering/ShaderData.h"

namespace Engine
{
	/*! \class OpenGLShader
	\brief API specific implementation of the shader (OpenGL)
	*/
	class OpenGLShader : public Shader
	{
	private:
		void cacheUniformsExtractLayout(const std::string& path); //!< Cache shader uniforms and extract its buffer layout \param path path to the shader file
		std::map<std::string, std::function<bool(void* data)>> m_dispatcher; //!< Set of uniforms and actions to be performed when uploading data

	public:
		OpenGLShader(const std::string& path); //!< Custom constructor \param path path to the shader file
		/*! Custom constructor
		\param vertexPath path to the vertex shader file
		\param fragmentPath path to the fragment shader file
		*/
		OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
		/*! Custom constructor
		\param vertexPath path to the vertex shader file
		\param fragmentPath path to the fragment shader file
		\param geometryPath path to the geometry shader file
		\param tessellationPath path to the tessellation shader file
		*/
		OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string & geometryPath, const std::string & tessellationControlPath, const std::string & tessellationEvalPath);

		// Inherited via Shader
		unsigned int getID() override; //!< Get unique shader ID
		void bind() override; //!< Use shader in the application
		void unbind() override; //!< Stop using shader in the application
		//! Compile shader code and link the shader in the memory
		void compileAndLink(const std::string& vertex, const std::string& fragment);
		//! Compile shader code and link the shader in the memory
		void compileAndLink(const std::string& vertex, const std::string& fragment, const std::string & geometry, const std::string & tessellationControl, const std::string & tessellationEval);
		/*! Feed shader with data
		/*! Feed shader with data 
		\param dataName name of the uniform 
		\param data data to be passed
		*/
		bool uploadData(const std::string & dataName, void * data) override;
		BufferLayout& getBufferLayout() override; //!< Get buffer layout of a shader
		void parseSource(const std::string& path); //!< Read shader code \param path path to the shader file
		std::map<std::string, std::pair<ShaderDataType, unsigned int>> getUniformCache() override; //!< Get the whole block of uniforms in the shader \return map of uniforms from the shader
		void checkCompileErrors(unsigned int shader, std::string type) override; //!< Check for compile errors.
	};
}