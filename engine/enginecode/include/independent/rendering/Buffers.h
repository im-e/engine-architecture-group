#pragma once

/*! \file Buffers.h
\brief Contains data and API agnostic buffers implementation
*/

#include "ShaderData.h"

#include <vector>

namespace Engine
{
	/*! \class BufferElement
	\brief An element of a buffer layout.
	
		This effectively means a BufferElement is a shader data
	*/
	class BufferElement
	{
	private:
		unsigned int m_size; //!< Size of an element
		unsigned int m_offset; //!< Offset from the beginning of the buffer to where all the associated buffer object data begins
		ShaderDataType m_shaderDataType; //!< Buffer's SDT
		bool m_normalized; //!< Is an element normalized?

	public:
		//! Default constructor
		BufferElement() {}; 
		//! Custom constructor \param type element's SDT \param normalised is an element normalised?
		BufferElement(ShaderDataType type, bool normalised = false) :
			m_shaderDataType(type), m_normalized(normalised), m_size(ShaderData::shaderDataTypeSize(type)), m_offset(0) {};

		//! Sets element's size \param size new size
		void setSize(unsigned int size) { m_size = size; }
		//! Returns element's size \return current size
		unsigned int getSize() { return m_size; }

		//! Sets element offset \param offset new offset
		void setOffset(unsigned int offset) { m_offset = offset; }
		//! Returns element's offset \return current offset
		unsigned int getOffset() { return m_offset; } 

		//! Returns element's SDT \return current shader data type
		ShaderDataType getDataType() { return m_shaderDataType; }
		//! Returns element's component count \return component count
		unsigned int getComponentCount() { return ShaderData::shaderDataTypeComponentCount(m_shaderDataType); }

		//! Is an element normalized? \return true if normalized
		bool isNormalized() { return m_normalized; }
	};

	/*! \class BufferLayout
	\brief Represents shader data
	*/
	class BufferLayout
	{
	private:
		unsigned int m_stride; //!< Byte offset from one vertex to the next one
		std::vector<BufferElement> m_elements; //!< Vector of elements in layout

		void calculateStrideAndOffset(); //!< Calculates stride and offset of a buffer

	public:
		//! Default constructor
		BufferLayout() {}; 
		BufferLayout(const std::initializer_list<BufferElement>& elements); //!< Custom constructor \param elements set of shader data
		//! Returns layout's stride \return layout's stride
		inline unsigned int getStride() const { return m_stride; } 

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); } //!< Marks beginning of buffer elements \return beginning of elements set
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); } //!< Marks beginning of buffer elements (unchangeable) \return beginning of elements set
		std::vector<BufferElement>::iterator end() { return m_elements.end(); } //!< Marks end of buffer elements \return end of elements set
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); } //!< Marks end of buffer elements (unchangeable) \return end of elements set

		inline const std::vector<BufferElement>& getElements() { return m_elements; } //!< Returns entire set of elements \return set of elements
		void addElement(ShaderDataType type); //!< Adds element to a layout
	};

	//Classes forward declarations
	class VertexBuffer;
	class IndexBuffer;

	/*! \class VertexArray
	\brief API agnostic VAO interface
	*/
	class VertexArray
	{
	protected:
		unsigned int m_arrayBuff; //!< VAO id
		unsigned int m_drawCount; //!< Number of draw calls
		std::shared_ptr<VertexBuffer> m_vBuff; //!< Pointer to vertex buffer
		std::shared_ptr<IndexBuffer> m_iBuff; //!< Pointer to index buffer

	public:
		virtual void bind() = 0; //!< Binds VAO
		virtual void unbind() = 0; //!< Unbinds VAO
		virtual void setVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuff) = 0; //!< Binds VBO to VAO \param vertexBuff new VBO
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuff) = 0; //!< Binds EBO to VAO \param indexBuff new EBO
		virtual std::shared_ptr<VertexBuffer> getVertexBuffer() const = 0; //!< Returns bound VBO \return VBO bound to VAO
		virtual std::shared_ptr<IndexBuffer> getIndexBuffer() const = 0; //!< Returns bound EBO \return EBO bound to VAO
		virtual void setDrawCount(unsigned int count) = 0; //!< Sets number of draw calls \param count new number of draws
		virtual unsigned int getDrawCount() const = 0; //!< Returns current number of draw calls \return number of draw calls
		virtual unsigned int getBuffer() const = 0; //!< Returns VAO id \return VAO id

		static VertexArray* create(); //!< Creates VAO
	};

	/*! \class VertexBuffer
	\brief API agnostic VBO interface
	*/
	class VertexBuffer
	{
	protected:
		unsigned int m_vertexBuff; //!< VBO id
		BufferLayout m_layout; //!< Layout bound to VBO

	public:
		virtual void bind() = 0; //!< Binds VBO
		virtual void unbind() = 0; //!< Unbinds VBO
		virtual void setLayout(const BufferLayout& layout) = 0; //!< Binds layout to a VBO \param layout new VBO layout (shader data)
		virtual const BufferLayout& getLayout() const = 0; //!< Returns currently bound layout \return current layout
		virtual void edit(float* vertices, unsigned int size, unsigned int offset) = 0; //!< Edit the data stored in the buffer

		static VertexBuffer* create(float* vertices, unsigned int size, BufferLayout& layout); //!< Creates VBO \param vertices number of vertices \param size size of vertices set \param layout shader data

		unsigned int getVertexBuff() { return m_vertexBuff; } //!< Returns VBO id \return VBO id
	};

	/*! \class IndexBuffer
	\brief API agnostic EBO implementation
	*/
	class IndexBuffer
	{
	protected:
		unsigned int m_indexBuff; //!< EBO id
		unsigned int* m_indices; //!< Number of indices
		unsigned int m_size; //!< Size of indices set

	public:
		virtual void bind() = 0; //!< Binds EBO
		virtual void unbind() = 0; //!< Unbinds EBO
		virtual unsigned int getSize() const = 0; //!< Returns size of EBO

		static IndexBuffer* create(unsigned int* indices, unsigned int size); //!< Creates EBO \param indices number of indices \param size size of indices set
		unsigned int getIndexBuff() const { return m_indexBuff; } //!< Returns EBO id \return EBO id

	};

	/*! \class UniformElement
	\brief Individual element in UniformLayout
	*/
	class UniformElement
	{
	private:
		unsigned int m_size; //!< Size of the element
		unsigned int m_offset; //!< Offset of the element
		ShaderDataType m_shaderDataType; //!< Type of the element
		bool m_normalized; //!< Is the element normalized?

		//! Aligns data in the element to certain size \param number data to be aligned \param alignment how big the alignment should be \return aligned number
		int align(int number, int alignment)
		{
			int remainder = number % alignment;
			if (remainder == 0) return number;

			return number + alignment - remainder;
		}

	public:
		//! Default constructor.
		UniformElement() {};
		//! Custom constructor. \param type type of the element \param normalised if the element is normalized
		UniformElement(ShaderDataType type, bool normalised = false) :
			m_shaderDataType(type), m_normalized(normalised), m_size(align(ShaderData::shaderDataTypeSize(type), 16)), m_offset(0) 
		{
		};

		//! Set new size of the element \param size new size
		void setSize(unsigned int size) { m_size = size; }
		//! Get current size of the element \return current size
		unsigned int getSize() { return m_size; }

		//! Set new offset of the element \param offset new offset
		void setOffset(unsigned int offset) { m_offset = offset; }
		//! Get current offset of the element \return current offset
		unsigned int getOffset() { return m_offset; }

		//! Get data type of the element \return shader data type
		ShaderDataType getDataType() { return m_shaderDataType; }
		//! Get how many components does the element have \return number of components
		unsigned int getComponentCount() { return ShaderData::shaderDataTypeComponentCount(m_shaderDataType); }

		//! Is the element normalized? \return if data is normalized
		bool isNormalized() { return m_normalized; }
	};

	/*! \class UniformLayout
	\brief Set of all elements in a layout
	*/
	class UniformLayout
	{
	private:
		unsigned int m_stride; //!< Byte offset from one element to the next one
		std::vector<UniformElement> m_elements; //!< Collection of elements

		//!< Calculates stride and offset of a buffer
		void calculateStrideAndOffset();

	public:
		//! Default constructor
		UniformLayout() {};
		UniformLayout(const std::initializer_list<UniformElement>& elements); //!< Custom constructor \param elements set of shader data
		 //! Returns layout's stride \return layout's stride
		inline unsigned int getStride() const { return m_stride; }

		std::vector<UniformElement>::iterator begin() { return m_elements.begin(); }//!< Marks beginning of buffer elements \return beginning of elements set
		std::vector<UniformElement>::const_iterator begin() const { return m_elements.begin(); }//!< Marks beginning of buffer elements (unchangeable) \return beginning of elements set
		std::vector<UniformElement>::iterator end() { return m_elements.end(); }//!< Marks end of buffer elements \return end of elements set
		std::vector<UniformElement>::const_iterator end() const { return m_elements.end(); } //!< Marks end of buffer elements (unchangeable) \return end of elements set

		inline const std::vector<UniformElement>& getElements() { return m_elements; }//!< Returns entire set of elements \return set of elements
		void addElement(ShaderDataType type);//!< Adds element to a layout
	};

	class Shader; //Shader class forward declaration to avoid include loop

	/*! \class UniformBuffer
	\brief Represents data common for objects in the scenes
	*/
	class UniformBuffer
	{
	protected:
		unsigned int m_uniformBuff; //!< Buffer's ID
		UniformLayout m_layout; //!< Buffer's layout

	public:
		virtual void bind() = 0; //!< Binds layout for usage
		virtual void unbind() = 0; //!< Unbinds layout
		//! Attaches block of data to the shader \param shader shader that is going to use the buffer \param blockName name of the data block in the shader
		virtual void attachShaderBlock(const std::shared_ptr<Shader>& shader, const std::string& blockName) = 0; 
		//! Set data in the data block \param offset where the data starts \param size size of the data \param data actual data
		virtual void setData(unsigned int offset, unsigned int size, void* data) = 0;
		//! Get layout associated with uniform buffer \return data layout
		virtual UniformLayout getLayout() const = 0;

		//! Create new UBO \param size uniform's buffer size \param layout layout associated with the UBO \return new UBO according to current render API
		static UniformBuffer* create(unsigned int size, UniformLayout& layout);
		//! Create new UBO \param size size of the UBO \param rangeStart point where UBO begins \param rangeEnd point where UBO ends \param layout layout associated with the UBO \return new UBO according to current render API
		static UniformBuffer* create(unsigned int size, unsigned int rangeStart, unsigned int rangeEnd, UniformLayout& layout);
	};
}