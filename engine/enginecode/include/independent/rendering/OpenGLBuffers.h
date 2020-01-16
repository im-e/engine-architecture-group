#pragma once

/*! \file OpenGLBuffers.h
\brief Contains implementation for GPU buffers used in OpenGL
*/

#include "rendering/Buffers.h"

namespace Engine
{
	/*! \class OpenGLVertexArray
	\brief Implementation of Vertex Array Object (VAO) used in OpenGL
	*/
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray(); //!< Default constructor

		// Inherited via VertexArray
		virtual void bind() override; //!< Binds VAO
		virtual void unbind() override; //!< Unbinds VAO
		virtual void setVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuff) override; //!< Sets VBO \param vertexBuff VBO to be set
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuff) override; //!< Sets EBO \param indexBuff EBO to be set
		virtual std::shared_ptr<VertexBuffer> getVertexBuffer() const override; //!< Gets set VBO \return currently bound VBO
		virtual std::shared_ptr<IndexBuffer> getIndexBuffer() const override; //!< Gets set EBO \return currently bound EBO
		virtual void setDrawCount(unsigned int count) override; //!< Sets number of draw calls \param count number of draw calls
		virtual unsigned int getDrawCount() const override; //!< Returns number of draw calls \return number of draw calls
		virtual unsigned int getBuffer() const override; //!< Returns VAO index \return current VAO index

		~OpenGLVertexArray(); //!< Default destructor
	};

	/*! \class OpenGLVertexBuffer
	\brief VBO implementation for OpenGL

	*/
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, unsigned int size, BufferLayout& layout); //!< Custom constructor \param vertices set of vertices to be draw \param size size of vertices set \param layout shader's layout (data)

		// Inherited via VertexBuffer
		void bind() override; //!< Binds VBO
		void unbind() override; //!< Unbinds vbo
		void setLayout(const BufferLayout& layout) override; //!< Sets layout
		const BufferLayout& getLayout() const override; //!< Gets current layout \return current layout

		~OpenGLVertexBuffer(); //!< Default destructor
	};

	/*! \class OpenGLIndexBuffer
	\brief Index buffer (EBO) implementation for OpenGL
	*/
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int* indices, unsigned int size); //!< Custom constructor \param indices set of indices \param size size of the indices set

		void bind() override; //!< Binds EBO
		void unbind() override; //!< Unbinds EBO
		unsigned int getSize() const override; //!< Gets EBO size \return size of the EBO

		~OpenGLIndexBuffer(); //!< Default destructor
	};

	/*! \class OpenGLUniformBuffer
	\brief Uniform buffer (UBO) implementation for OpenGL.
	*/
	class OpenGLUniformBuffer : public UniformBuffer
	{
	private:
		unsigned int m_bindingPoint; //!< UBO's binding point
		static unsigned int s_bindingIndex; //!< Current binding index for UBOs
	public:
		//! Custom constructor. \param size size of the UBO \param layout layout associated with the UBO
		OpenGLUniformBuffer(unsigned int size, UniformLayout& layout); 
		//! Custom constructor. \param size size of the UBO \param rangeStart point where UBO begins \param rangeEnd point where UBO ends \param layout layout associated with the UBO
		OpenGLUniformBuffer(unsigned int size, unsigned int rangeStart, unsigned int rangeEnd, UniformLayout& layout);

		// Inherited via UniformBuffer
		virtual void bind() override;
		virtual void unbind() override;
		virtual void attachShaderBlock(const std::shared_ptr<Shader>& shader, const std::string & blockName) override;
		virtual void setData(unsigned int offset, unsigned int size, void * data) override;
		virtual UniformLayout getLayout() const override;
	};
}