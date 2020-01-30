#include "engine_pch.h"
#include "resources/OpenGLShader.h"

#include <glad/glad.h>
#include "core/application.h"

namespace Engine
{
	OpenGLShader::OpenGLShader(const std::string & path)
	{
		parseSource(path);
	}

	OpenGLShader::OpenGLShader(const std::string & vertexPath, const std::string & fragmentPath)
	{
		std::string vCode, fCode; //shader codes

		std::ifstream vFile, fFile; //shaderFiles

		//open files
		vFile.open(vertexPath);
		fFile.open(fragmentPath);

		//read files' content and close them
		std::stringstream vStream, fStream;

		vStream << vFile.rdbuf();
		fStream << fFile.rdbuf();

		vFile.close();
		fFile.close();

		//'feed' the code
		vCode = vStream.str();
		fCode = fStream.str();

		const char* vShader = vCode.c_str();
		const char* fShader = fCode.c_str();

		compileAndLink(vShader, fShader);
		cacheUniformsExtractLayout(vShader);
		cacheUniformsExtractLayout(fShader);
	}

	unsigned int OpenGLShader::getID()
	{
		return m_ID;
	}

	void OpenGLShader::bind()
	{
		glUseProgram(m_ID);
	}

	void OpenGLShader::unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::compileAndLink(const std::string & vertex, const std::string & fragment)
	{
		//vertex shader
		m_vertID = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = vertex.c_str(); //source code
		glShaderSource(m_vertID, 1, &source, 0);
		glCompileShader(m_vertID);

		GLint isCompiled = 0;
		glGetShaderiv(m_vertID, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE) //if failed to compile
		{
			GLint maxLength = 0;
			glGetShaderiv(m_vertID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(m_vertID, maxLength, &maxLength, infoLog.data());
			LogError("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(m_vertID);
			return;
		}

		//fragment shader
		m_fragID = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragment.c_str();
		glShaderSource(m_fragID, 1, &source, 0);
		glCompileShader(m_fragID);

		glGetShaderiv(m_fragID, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(m_fragID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(m_fragID, maxLength, &maxLength, infoLog.data());
			LogError("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(m_fragID);
			glDeleteShader(m_vertID);

			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, m_vertID);
		glAttachShader(m_ID, m_fragID);
		glLinkProgram(m_ID);

		GLint isLinked = 0; //success?
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE) //if failed to link
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ID, maxLength, &maxLength, &infoLog[0]);
			LogError("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_ID);
			glDeleteShader(m_vertID);
			glDeleteShader(m_fragID);

			return;
		}

		//delete shaders as no longer needed
		glDetachShader(m_ID, m_vertID);
		glDetachShader(m_ID, m_fragID);
	}

	void OpenGLShader::parseSource(const std::string & path)
	{
		std::fstream handleFile(path, std::ios::in);
		enum { None = -1, Vertex = 0, Fragment = 1 } regions;

		if (handleFile.is_open() == false)
			LogError("Could not open file at {0}", path);

		std::string line, source[2];

		while (getline(handleFile, line))
		{
			if (line.find("#region Vertex") != std::string::npos) { regions = Vertex;		continue; }
			if (line.find("#region Fragment") != std::string::npos) { regions = Fragment;	continue; }
			if (line.find("#region Geometry") != std::string::npos) { regions = None;		continue; }
			if (line.find("#region Tesselation") != std::string::npos) { regions = None;		continue; }

			if (regions != None) source[regions] += (line + "\n");
		}

		handleFile.close();

		Application::getInstance().getWindow()->getGraphicsContext()->swapToCurrentThread();
		compileAndLink(source[Vertex], source[Fragment]);

		cacheUniformsExtractLayout(path);
		Application::getInstance().getWindow()->getGraphicsContext()->unbindCurrentThread();
	}

	void OpenGLShader::cacheUniformsExtractLayout(const std::string& path)
	{
		std::fstream handleFile(path, std::ios::in);

		if (handleFile.is_open() == false)
			LogError("Could not open {0}", path);

		std::string line, uni, type, name;

		while (getline(handleFile, line))
		{
			if (line.find("uniform") != std::string::npos)
			{
				std::stringstream ss(line);
				ss >> uni;
				ss >> type;
				ss >> name;
				if(name.at(name.length()-1) == ';')
					name.pop_back(); //remove semicolon

				unsigned int location = glGetUniformLocation(m_ID, name.c_str());

				//add to uniform cache map
				m_uniformCache[name] = std::pair<ShaderDataType, unsigned int>(ShaderData::stringToDataType(type), location);

				switch(ShaderData::stringToDataType(type))
				{
				case ShaderDataType::None:
					m_dispatcher[name] = [](void* data)
					{
						return false;
					};
					break;
				case ShaderDataType::Int:				
					m_dispatcher[name] = [location](void* data)
					{	
						glUniform1i(location, (GLint)data);
						return true;
					};
					break;
				case ShaderDataType::Int2:
					m_dispatcher[name] = [location](void* data)
					{
						glUniform2iv(location, 1, (GLint*)data);
						return true;
					};
					break;
				case ShaderDataType::Int3:
					m_dispatcher[name] = [location](void* data)
					{
						glUniform3iv(location, 1, (GLint*)data);
						return true;
					};
					break;
				case ShaderDataType::Int4:
					m_dispatcher[name] = [location](void* data)
					{
						glUniform4iv(location, 1, (GLint*)data);
						return true;
					};
					break;
				case ShaderDataType::Float:
					m_dispatcher[name] = [location](void* data)
					{
						GLfloat *t = (GLfloat*)data; //cast to float pointer, because OpenGL did not like a classic c-casting to float (memory addresses are not of type FLOAT)
						
						glUniform1i(location, *t);
						return true;
					};
					break;
				case ShaderDataType::Float2:
					m_dispatcher[name] = [location](void* data)
					{
						glUniform2fv(location, 1, (GLfloat*)data);
						return true;
					};
					break;
				case ShaderDataType::Float3:
					m_dispatcher[name] = [location](void* data)
					{
						glUniform3fv(location, 1, (GLfloat*)data);
						return true;
					};
					break;
				case ShaderDataType::Float4:
					m_dispatcher[name] = [location](void* data)
					{
						glUniform4fv(location, 1, (GLfloat*)data);
						return true;
					};
					break;
				case ShaderDataType::Mat2:
					m_dispatcher[name] = [location](void* data)
					{
						glUniformMatrix2fv(location, 1, GL_FALSE, (GLfloat*)data);
						return true;
					};
					break;
				case ShaderDataType::Mat3:
					m_dispatcher[name] = [location](void* data)
					{
						glUniformMatrix3fv(location, 1, GL_FALSE, (GLfloat*)data);
						return true;
					};
					break;
				case ShaderDataType::Mat4:
					m_dispatcher[name] = [location](void* data)
					{
						glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)data);
						return true;
					};
					break;
				case ShaderDataType::Sampler2D:
					m_dispatcher[name] = [location](void* data)
					{
						glUniform1i(location, (GLuint)data);
						return true;
					};
					break;
				case ShaderDataType::Bool:
					m_dispatcher[name] = [location](void* data)
					{
						glUniform1i(location, (GLboolean)data);
						return true;
					};
					break;
				default:
					m_dispatcher[name] = [](void* data)
					{
						return false;
					};
				}
			}

			//extract buffer layouts
			if (line.find("layout") != std::string::npos)
			{
				std::size_t pos = line.find("in");

				//exit loop if layout is of keyword OUT
				if (pos > line.length())
					continue;

				line.erase(0, pos + 3);

				std::stringstream ss(line);
				ss >> type;

				switch (ShaderData::stringToDataType(type))
				{
				case ShaderDataType::None:
					LogError("Layout error");
					break;
				case ShaderDataType::Int:
					m_bufferLayout.addElement(ShaderDataType::Int);
					break;
				case ShaderDataType::Int2:
					m_bufferLayout.addElement(ShaderDataType::Int2);
					break;
				case ShaderDataType::Int3:
					m_bufferLayout.addElement(ShaderDataType::Int3);
					break;
				case ShaderDataType::Int4:
					m_bufferLayout.addElement(ShaderDataType::Int4);
					break;
				case ShaderDataType::Float:
					m_bufferLayout.addElement(ShaderDataType::Float);
					break;
				case ShaderDataType::Float2:
					m_bufferLayout.addElement(ShaderDataType::Float2);
					break;
				case ShaderDataType::Float3:
					m_bufferLayout.addElement(ShaderDataType::Float3);
					break;
				case ShaderDataType::Float4:
					m_bufferLayout.addElement(ShaderDataType::Float4);
					break;
				case ShaderDataType::Mat2:
					m_bufferLayout.addElement(ShaderDataType::Mat2);
					break;
				case ShaderDataType::Mat3:
					m_bufferLayout.addElement(ShaderDataType::Mat3);
					break;
				case ShaderDataType::Mat4:
					m_bufferLayout.addElement(ShaderDataType::Mat4);
					break;
				case ShaderDataType::Sampler2D:
					m_bufferLayout.addElement(ShaderDataType::Sampler2D);
					break;
				case ShaderDataType::Bool:
					m_bufferLayout.addElement(ShaderDataType::Bool);
					break;
				default:
					LogError("Unknown layout type");
					break;
				}
			}
		}

		handleFile.close();
	}

	bool OpenGLShader::uploadData(const std::string & dataName, void * data)
	{
		return m_dispatcher[dataName](data);
	}

	BufferLayout OpenGLShader::getBufferLayout() const
	{
		return m_bufferLayout;
	}

	std::map<std::string, std::pair<ShaderDataType, unsigned int>> OpenGLShader::getUniformCache()
	{
		return m_uniformCache;
	}
	
}