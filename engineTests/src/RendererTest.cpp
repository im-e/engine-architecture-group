#include "RendererTest.h"

TEST(UniformBuffer, CreateUBO)
{
	ResourceManagerInstance->addUBO("testUBO", layout.getStride(), layout);

	EXPECT_NE(nullptr, ResourceManagerInstance->getUBO().getAsset("testUBO").get());
}

TEST(UniformBuffer, AlignmentTo16)
{
	EXPECT_EQ(16, uboElement.getSize());
}

TEST(UniformBuffer, GetLayoutContent)
{

	Engine::UniformLayout tempLayout = ResourceManagerInstance->getUBO().getAsset("testUBO")->getLayout();

	for (std::vector<Engine::UniformElement>::iterator it = layout.begin(); it != layout.end(); ++it)
	{
		for (std::vector<Engine::UniformElement>::iterator tempIt = tempLayout.begin(); tempIt != tempLayout.end(); ++tempIt)
		{
			EXPECT_EQ(it->getDataType(), tempIt->getDataType());
		}
	}
}
TEST(UniformBuffer, AddElement)
{
	layout.addElement(Engine::ShaderDataType::Sampler2D);

	EXPECT_NE(2, layout.getElements().size());
}

TEST(Material, CreateMaterial)
{
	ResourceManagerInstance->addShader("rendererTestShader", "assets/shaders/flatColour.glsl");
	ResourceManagerInstance->addVAO("rendererTestVAO");

	ResourceManagerInstance->addMaterial("testMaterial",
		ResourceManagerInstance->getShader().getAsset("rendererTestShader"),
		ResourceManagerInstance->getVAO().getAsset("rendererTestVAO"));

	EXPECT_NE(nullptr, ResourceManagerInstance->getMaterial().getAsset("testMaterial").get());
}

TEST(Material, GetShader)
{
	EXPECT_EQ(ResourceManagerInstance->getShader().getAsset("rendererTestShader").get(),
		ResourceManagerInstance->getMaterial().getAsset("testMaterial")->getShader().get());
}

TEST(Material, GetGeometry)
{
	auto testVAO = 
		static_cast<Engine::VertexArray*>(ResourceManagerInstance->getMaterial().getAsset("testMaterial")->getGeometry());

	EXPECT_EQ(ResourceManagerInstance->getVAO().getAsset("rendererTestVAO")->getBuffer(), testVAO->getBuffer());
}

TEST(Renderer, CreateRenderer2D)
{
	m_renderer2D.reset(Engine::Renderer::create2D());
	EXPECT_NE(m_renderer2D.get(), nullptr);
}

TEST(Renderer, CreateRenderer3D)
{
	m_renderer3D.reset(Engine::Renderer::create3D());
	EXPECT_NE(m_renderer3D.get(), nullptr);
}