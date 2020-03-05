#include "engine_pch.h"
#include "rendering/OpenGLBuffers.h"

#include "glad/glad.h"
#include "resources/Shader.h"

namespace Engine
{
	/**********************************
				Vertex Array
	***********************************/
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_arrayBuff);
		glBindVertexArray(m_arrayBuff);
	}

	void OpenGLVertexArray::bind()
	{
		glBindVertexArray(m_arrayBuff);
		m_vBuff->bind();
		m_iBuff->bind();
	}

	void OpenGLVertexArray::unbind()
	{
		glBindVertexArray(0);
		m_vBuff->unbind();
		m_iBuff->unbind();
	}

	void OpenGLVertexArray::setVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuff)
	{
		m_vBuff = vertexBuff;
		m_vBuff->bind();
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuff)
	{
		m_iBuff = indexBuff;
		m_iBuff->bind();
		m_drawCount = m_iBuff->getSize();
	}

	std::shared_ptr<VertexBuffer> OpenGLVertexArray::getVertexBuffer() const
	{
		return m_vBuff;
	}

	std::shared_ptr<IndexBuffer> OpenGLVertexArray::getIndexBuffer() const
	{
		return m_iBuff;
	}

	void OpenGLVertexArray::setDrawCount(unsigned int count)
	{
		m_drawCount = count;
	}

	unsigned int OpenGLVertexArray::getDrawCount() const
	{
		return m_drawCount;
	}

	unsigned int OpenGLVertexArray::getBuffer() const
	{
		return m_arrayBuff;
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteBuffers(1, &m_arrayBuff);
		delete this;
	}

	/**********************************
				Vertex buffer
	***********************************/

	OpenGLVertexBuffer::OpenGLVertexBuffer(float * vertices, unsigned int size, BufferLayout & layout)
	{
		glCreateBuffers(1, &m_vertexBuff);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuff);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
		m_layout = layout;

		unsigned int index = 0;
		for (std::vector<BufferElement>::iterator it = layout.begin(); it != layout.end(); ++it)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				it->getComponentCount(),
				ShaderData::shaderDataTypeToOpenGLType(it->getDataType()),
				it->isNormalized(),
				layout.getStride(),
				(const void*)it->getOffset());
			index++;
		}
	}

	void OpenGLVertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuff);
	}

	void OpenGLVertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::setLayout(const BufferLayout& layout)
	{
		m_layout = layout;
	}

	void OpenGLVertexBuffer::edit(float* vertices, unsigned int size, unsigned int offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuff);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const BufferLayout& OpenGLVertexBuffer::getLayout() const
	{
		return m_layout;
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_vertexBuff);
		delete this;
	}

	/**********************************
				Index buffer
	***********************************/
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, unsigned int size)
	{
		m_indices = indices;
		m_size = size;
		
		glCreateBuffers(1, &m_indexBuff);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuff);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, m_indices, GL_DYNAMIC_DRAW);
	}

	void OpenGLIndexBuffer::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuff);
	}

	void OpenGLIndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int OpenGLIndexBuffer::getSize() const
	{
		return m_size;
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_indexBuff);
		delete this;
	}
	
	/**********************************
				Uniform buffer
	***********************************/

	unsigned int OpenGLUniformBuffer::s_bindingIndex = 0;

	OpenGLUniformBuffer::OpenGLUniformBuffer(unsigned int size, UniformLayout & layout)
	{
		m_bindingPoint = s_bindingIndex;
		s_bindingIndex++;
		glGenBuffers(1, &m_uniformBuff);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuff);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, m_bindingPoint, m_uniformBuff, 0, size);
		m_layout = layout;
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(unsigned int size, unsigned int rangeStart, unsigned int rangeEnd, UniformLayout & layout)
	{
		m_bindingPoint = s_bindingIndex;
		s_bindingIndex++;
		glGenBuffers(1, &m_uniformBuff);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuff);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, m_bindingPoint, m_uniformBuff, rangeStart, rangeEnd);
		m_layout = layout;
	}

	void OpenGLUniformBuffer::bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuff);
	}

	void OpenGLUniformBuffer::unbind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void OpenGLUniformBuffer::attachShaderBlock(const std::shared_ptr<Shader>& shader, const std::string & blockName)
	{
		unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader->getID(), blockName.c_str());
		glUniformBlockBinding(shader->getID(), uniformBlockIndex, m_bindingPoint);
	}

	void OpenGLUniformBuffer::setData(unsigned int offset, unsigned int size, void * data)
	{
		bind();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

	UniformLayout OpenGLUniformBuffer::getLayout() const
	{
		return m_layout;
	}

}