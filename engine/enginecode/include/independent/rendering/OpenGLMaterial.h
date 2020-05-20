#pragma once

/*! \file OpenGLMaterial.h
\brief Implementation of materials for OpenGL
*/

#include "Material.h"

namespace Engine
{
	/*! \class OpenGLMaterial
	\brief Provides implementation of materials from OpenGL. Inherits from material.
	*/
	class OpenGLMaterial : public Material
	{
	private:
		std::shared_ptr<Shader> m_shader; //!< Current material's shader
		std::shared_ptr<VertexArray> m_VAO; //!< Current material's geometry
		std::map<std::string, void *> m_data; //!< Current material's data
		bool m_tessFlag; //!< Should rendered material be tessellated?

	public:
		OpenGLMaterial(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO, bool tess = false); //!< Custom constructor. \param shader default shader \param VAO default geometry

		// Inherited via Material
		void setShader(const std::shared_ptr<Shader>& shader) override;
		void setShaderDataBlock(const std::map<std::string,void*>& data) override;
		void setShaderDataElement(const std::string & dataName, void * data) override;
		void setVertexData(float * vertices, unsigned int size, unsigned int offset) override;
		std::shared_ptr<Shader> getShader() override;
		void setGeometry(void * geometry) override;
		void * getGeometry() override;
		bool getTessFlag() override;
		std::map<std::string, void*> getData() override;
	};
}