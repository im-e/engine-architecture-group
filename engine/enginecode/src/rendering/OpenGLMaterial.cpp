#include "engine_pch.h"
#include "rendering/OpenGLMaterial.h"

namespace Engine
{
	OpenGLMaterial::OpenGLMaterial(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO)
	{
		m_shader = shader;
		m_VAO = VAO;
		m_geometry = (void *) VAO.get();
	}

	void OpenGLMaterial::setShader(const std::shared_ptr<Shader>& shader)
	{
		m_shader = shader;
	}

	void OpenGLMaterial::setGeometry(void * geometry)
	{
		m_geometry = geometry;
	}
	
	void OpenGLMaterial::setShaderDataBlock(const std::map<std::string, void*>& data)
	{
		m_data = data;
	}

	void OpenGLMaterial::setShaderDataElement(const std::string & dataName, void * data)
	{
		m_data[dataName] = data;
	}

	void OpenGLMaterial::setVertexData(float * vertices, unsigned int size, unsigned int offset)
	{
	}

	std::shared_ptr<Shader> OpenGLMaterial::getShader()
	{
		return m_shader;
	}

	void * OpenGLMaterial::getGeometry()
	{
		return m_geometry;
	}

	std::map<std::string, void*> OpenGLMaterial::getData()
	{
		return m_data;
	}
}