#include "engine_pch.h"

#include "rendering/RenderAPI.h"
#include "rendering/Buffers.h"
#include "rendering/OpenGLBuffers.h"
#include "src/rendering/RenderAPI.cpp"

namespace Engine
{
	/**********************************
				BufferLayouts
	***********************************/
	void BufferLayout::calculateStrideAndOffset()
	{
		unsigned int previousOffset = 0;
		m_stride = 0;

		for (BufferElement& element : m_elements)
		{
			element.setOffset(previousOffset);
			previousOffset += element.getSize();
		}

		m_stride = previousOffset;
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) : m_elements(elements)
	{
		calculateStrideAndOffset();
	}

	void BufferLayout::addElement(ShaderDataType type)
	{
		m_elements.push_back(BufferElement(type));
		calculateStrideAndOffset();
	}

	/**********************************
				Array buffer
	***********************************/
	VertexArray * VertexArray::create()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexArray();
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		default:
			LogError("Unknown graphics API - please specify");
			return nullptr;
			break;
		}
	}

	/**********************************
				Vertex buffer
	***********************************/
	VertexBuffer * VertexBuffer::create(float * vertices, unsigned int size, BufferLayout & layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size, layout);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		default:
			LogError("Unknown graphics API - please specify");
			return nullptr;
			break;
		}
	}

	/**********************************
				Index buffer
	***********************************/
	IndexBuffer * IndexBuffer::create(unsigned int * indices, unsigned int size)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		default:
			LogError("Unknown graphics API - please specify");
			return nullptr;
			break;
		}
	}

	/**********************************
				Uniform buffer
	***********************************/
	UniformBuffer * UniformBuffer::create(unsigned int size, UniformLayout & layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLUniformBuffer(size, layout);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		default:
			LogError("Unknown graphics API - please specify");
			return nullptr;
			break;
		}
	}

	UniformBuffer * UniformBuffer::create(unsigned int size, unsigned int rangeStart, unsigned int rangeEnd, UniformLayout & layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLUniformBuffer(size, rangeStart, rangeEnd, layout);
			break;
		case RenderAPI::API::Direct3D:
			LogError("Direct3D not yet supported!");
			break;
		default:
			LogError("Unknown graphics API - please specify");
			return nullptr;
			break;
		}
	}

	UniformLayout::UniformLayout(const std::initializer_list<UniformElement>& elements) : m_elements(elements)
	{
		calculateStrideAndOffset();
	}

	void UniformLayout::calculateStrideAndOffset()
	{
		unsigned int previousOffset = 0;
		m_stride = 0;
		
		for (UniformElement& element : m_elements)
		{
			element.setOffset(previousOffset);
			previousOffset += element.getSize();
		}

		m_stride = previousOffset;
	}

	void UniformLayout::addElement(ShaderDataType type)
	{
		m_elements.push_back(UniformElement(type));
		calculateStrideAndOffset();
	}

}