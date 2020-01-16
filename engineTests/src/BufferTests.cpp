#include "BufferTests.h"
#include "glad/glad.h"

TEST(ShaderData, DataSize)
{
	//0
	int size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::None);
	EXPECT_EQ(0, size);

	//4
	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Bool);
	EXPECT_EQ(sizeof(bool), size);

	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Int);
	EXPECT_EQ(sizeof(int), size);

	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Float);
	EXPECT_EQ(sizeof(float), size);

	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Sampler2D);
	EXPECT_EQ(sizeof(bool), size);

	//8
	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Float2);
	EXPECT_EQ(sizeof(float) * 2, size);

	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Int2);
	EXPECT_EQ(sizeof(int) * 2, size);

	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Mat2);
	EXPECT_EQ(sizeof(glm::mat2), size);

	//12
	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Int3);
	EXPECT_EQ(sizeof(int) * 3, size);

	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Float3);
	EXPECT_EQ(sizeof(float) * 3, size);

	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Mat3);
	EXPECT_EQ(sizeof(glm::mat3), size);

	//16
	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Float4);
	EXPECT_EQ(sizeof(int) * 4, size);

	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Int4);
	EXPECT_EQ(sizeof(int) * 4, size);

	size = shaderData.shaderDataTypeSize(Engine::ShaderDataType::Mat4);
	EXPECT_EQ(sizeof(glm::mat4), size);
}
TEST(ShaderData, ShaderDataComponentCount)
{
	//0
	int componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::None);
	EXPECT_EQ(0, componentCount);

	//1
	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Bool);
	EXPECT_EQ(1, componentCount);

	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Int);
	EXPECT_EQ(1, componentCount);

	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Float);
	EXPECT_EQ(1, componentCount);

	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Sampler2D);
	EXPECT_EQ(1, componentCount);

	//2
	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Float2);
	EXPECT_EQ(2, componentCount);

	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Int2);
	EXPECT_EQ(2, componentCount);

	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Mat2);
	EXPECT_EQ(2, componentCount);

	//3
	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Int3);
	EXPECT_EQ(3, componentCount);

	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Float3);
	EXPECT_EQ(3, componentCount);

	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Mat3);
	EXPECT_EQ(3, componentCount);

	//4
	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Float4);
	EXPECT_EQ(4, componentCount);

	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Int4);
	EXPECT_EQ(4, componentCount);

	componentCount = shaderData.shaderDataTypeComponentCount(Engine::ShaderDataType::Mat4);
	EXPECT_EQ(4, componentCount);
}
TEST(ShaderData, DataToOpenGL)
{
	GLenum GLType = 0;

	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::None);
	EXPECT_EQ(0, GLType);

	//GL_INT
	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Int);
	EXPECT_EQ(GL_INT, GLType);

	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Int2);
	EXPECT_EQ(GL_INT, GLType);

	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Int3);
	EXPECT_EQ(GL_INT, GLType);

	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Int4);
	EXPECT_EQ(GL_INT, GLType);

	//GL_FLOAT
	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Float);
	EXPECT_EQ(GL_FLOAT, GLType);

	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Float2);
	EXPECT_EQ(GL_FLOAT, GLType);

	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Float3);
	EXPECT_EQ(GL_FLOAT, GLType);

	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Float4);
	EXPECT_EQ(GL_FLOAT, GLType);

	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Mat2);
	EXPECT_EQ(GL_FLOAT, GLType);

	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Mat3);
	EXPECT_EQ(GL_FLOAT, GLType);

	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Mat4);
	EXPECT_EQ(GL_FLOAT, GLType);

	//GL_BOOL
	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Bool);
	EXPECT_EQ(GL_BOOL, GLType);

	//GL_SAMPLER_2D
	GLType = shaderData.shaderDataTypeToOpenGLType(Engine::ShaderDataType::Sampler2D);
	EXPECT_EQ(GL_SAMPLER_2D, GLType);
}
TEST(ShaderData, StringToType)
{
	std::string strType = "";
	Engine::ShaderDataType dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::None);

	strType = "bool";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Bool);

	strType = "int";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Int);

	strType = "ivec2";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Int2);

	strType = "ivec3";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Int3);

	strType = "ivec4";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Int4);

	strType = "float";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Float);

	strType = "vec2";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Float2);

	strType = "vec3";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Float3);

	strType = "vec4";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Float4);

	strType = "mat2";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Mat2);

	strType = "mat3";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Mat3);

	strType = "mat4";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Mat4);

	strType = "sampler2D";
	dataType = shaderData.stringToDataType(strType);
	EXPECT_EQ(dataType, Engine::ShaderDataType::Sampler2D);
}

TEST(BufferLayout, Initialization)
{
	EXPECT_EQ(2, layout.getElements().size());
}
TEST(BufferLayout, AddElement)
{
	layout.addElement(Engine::ShaderDataType::Sampler2D);

	EXPECT_NE(2, layout.getElements().size());
}
TEST(BufferLayout, StrideCalculation) 
{
	int stride = 0;

	for (std::vector<Engine::BufferElement>::iterator it = layout.begin(); it != layout.end(); ++it)
	{
		stride += it->getSize();
	}

	EXPECT_EQ(stride, layout.getStride());
}

TEST(RenderAPI, OpenGLInitialization)
{
	EXPECT_NE(Engine::RenderAPI::getAPI(), Engine::RenderAPI::API::None);
	EXPECT_NE(Engine::RenderAPI::getAPI(), Engine::RenderAPI::API::Direct3D);
	EXPECT_EQ(Engine::RenderAPI::getAPI(), Engine::RenderAPI::API::OpenGL);
}

TEST(Buffers, CreateVAO)
{

	m_vertexArray.reset(Engine::VertexArray::create());
	EXPECT_NE(nullptr, m_vertexArray.get());

}
TEST(Buffers, GetVAO)
{

	m_VAOs.push_back(m_vertexArray);

	m_vertexArray2.reset(Engine::VertexArray::create());
	m_VAOs.push_back(m_vertexArray2);

	EXPECT_EQ(m_vertexArray->getBuffer(), m_VAOs.at(0)->getBuffer());

}
TEST(Buffers, GetVBOGetEBO)
{

	m_vertexBuffer.reset(Engine::VertexBuffer::create(vertices, sizeof(vertices), layout));
	m_indexBuffer.reset(Engine::IndexBuffer::create(indices, sizeof(indices)));

	m_vertexArray->setVertexBuffer(m_vertexBuffer);
	m_vertexArray->setIndexBuffer(m_indexBuffer);

	EXPECT_NE(nullptr, m_vertexArray->getVertexBuffer().get());
	EXPECT_NE(nullptr, m_vertexArray->getIndexBuffer().get());

	EXPECT_EQ(m_vertexBuffer.get(), m_vertexArray->getVertexBuffer().get());
	EXPECT_EQ(m_indexBuffer.get(), m_vertexArray->getIndexBuffer().get());
	
}
TEST(Buffers, GetDrawCount)
{

	EXPECT_EQ(m_vertexArray->getDrawCount(), m_indexBuffer->getSize());

}

TEST(Buffers, CreateVBO)
{

	m_vertexBuffer.reset(Engine::VertexBuffer::create(vertices, sizeof(vertices), layout));
	EXPECT_NE(nullptr, m_vertexBuffer.get());

}
TEST(Buffers, GetLayoutContent)
{
	Engine::BufferLayout tempLayout = m_vertexBuffer->getLayout();
	for (std::vector<Engine::BufferElement>::iterator it = layout.begin(); it != layout.end(); ++it)
	{
		for (std::vector<Engine::BufferElement>::iterator tempIt = tempLayout.begin(); tempIt != tempLayout.end(); ++tempIt)
		{
			EXPECT_EQ(it->getDataType(), tempIt->getDataType());
		}
	}

}
TEST(Buffers, BindNewLayout)
{

	Engine::BufferLayout lay = 
	{
		Engine::ShaderDataType::Bool
	};

	m_vertexBuffer->setLayout(lay);
	
	Engine::BufferLayout tempLayout = m_vertexBuffer->getLayout();
	for (std::vector<Engine::BufferElement>::iterator it = layout.begin(); it != layout.end(); ++it)
	{
		for (std::vector<Engine::BufferElement>::iterator tempIt = tempLayout.begin(); tempIt != tempLayout.end(); ++tempIt)
		{
			EXPECT_NE(it->getDataType(), tempIt->getDataType());
		}
	}

}
TEST(Buffers, GetVBO)
{

	unsigned int vboID = m_vertexBuffer->getVertexBuff();
	EXPECT_EQ(m_vertexBuffer->getVertexBuff(), vboID);

}

TEST(Buffers, CreateEBO)
{

	m_indexBuffer.reset(Engine::IndexBuffer::create(indices, sizeof(indices)));
	EXPECT_NE(nullptr, m_indexBuffer.get());

}
TEST(Buffers, GetEBO)
{
	unsigned int eboID = m_indexBuffer->getIndexBuff();
	EXPECT_EQ(m_indexBuffer->getIndexBuff(), eboID);
}
TEST(Buffers, EBOSize)
{
	unsigned int size = sizeof(indices);
	EXPECT_EQ(size, m_indexBuffer->getSize());
}