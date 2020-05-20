#include "ComponentTests.h"

TEST(GameObject, CreateGO)
{
	m_go = std::make_shared<Engine::GameObject>(Engine::GameObject("TestGO"));

	EXPECT_NE(m_go.get(), nullptr);
}

TEST(Component, MaterialComponent)
{
	ResourceManagerInstance->addVAO("TestGOVAO");
	ResourceManagerInstance->addShader("TestGOShader", "assets/shaders/flatColour.glsl");

	ResourceManagerInstance->addMaterial("TestGOMat",
		ResourceManagerInstance->getShader().getAsset("TestGOShader"),
		ResourceManagerInstance->getVAO().getAsset("TestGOVAO"));

	m_materialComponent = std::make_shared<Engine::MaterialComponent>
		(Engine::MaterialComponent(ResourceManagerInstance->getMaterial().getAsset("TestGOMat")));

	EXPECT_NE(m_materialComponent.get(), nullptr);
}

TEST(Component, AddComponent)
{
	//int i = 0;
	//
	//m_go->addComponent(m_materialComponent);
	//
	//for (auto& c : m_go->getComponents())
	//{
	//	i++;
	//}
	//
	//EXPECT_NE(0, i);
}

TEST(Component, CheckDispatchTable)
{
	//EXPECT_NE(m_go->getMap().size(), 0);
}

TEST(Component, ReceiveMessage)
{
	//std::pair<std::string, void*> data("", 0);
	//Engine::ComponentMessage msg = Engine::ComponentMessage(Engine::ComponentMessageType::UniformSet, data);
	//
	//EXPECT_EQ(m_materialComponent->receiveMessage(msg), true);
}