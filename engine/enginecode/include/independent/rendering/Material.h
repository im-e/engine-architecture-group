#pragma once

/*! \file Material.h
\brief API agnostic implementation of material system in the engine in game
*/

#include <map>
#include <memory>

#include "Buffers.h"
#include "resources/Shader.h"

namespace Engine
{
	/*! \class Material
	\brief implements materials for the engine
	*/
	class Material
	{
	protected:
		void * m_geometry; //!< Geometry of the shader. Can be either VAO (OpenGL) or VBO (Non OpenGL)

	public:
		//! Binds new shader to the material \param shader new shader of the material
		virtual void setShader(const std::shared_ptr<Shader>& shader) = 0;
		//! Binds new geometry to the material \param geometry new geometry of the material
		virtual void setGeometry(void * geometry) = 0;
		//! Sets full data block from a shader \param data data from the shader
		virtual void setShaderDataBlock(const std::map<std::string, void*>& data) = 0;
		//! Sets an individual element of data in a shader \param dataName name of the uniform \param data actual data
		virtual void setShaderDataElement(const std::string& dataName, void* data) = 0;
		//! Sets vertex data of the material \param vertices new set of vertices \param size new size \param offset new offset
		virtual void setVertexData(float* vertices, unsigned int size, unsigned int offset) = 0;

		virtual std::shared_ptr<Shader> getShader() = 0; //!< Get shader associated with the material \return material's shader
		virtual std::map<std::string, void*> getData() = 0; //!< Get data of the material \return material's data
		virtual void * getGeometry() = 0; //!< Get geometry associated with the material \return geometry of the material
		virtual bool getTessFlag() = 0; //!< Get the tessellation flag.

		/*! Creates new material
		\param shader shader to be associated with the material
		\param VAO geometry to be associated with the material
		*/
		static Material* create(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO, bool tess = false);
		/*! Creates new material
		\param shader shader to be associated with the material
		\param VBO geometry to be associated with the material
		*/
		static Material* create(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexBuffer>& VBO, bool tess = false);
	};
}