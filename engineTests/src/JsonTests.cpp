#include "JsonTests.h"

TEST(DataTest, PushLayer)
{
	m_layerStack.reset(new Engine::LayerStack());
	m_layerStack->start(Engine::SystemSignal::None);

	m_layerStack->push(std::make_shared<Engine::JsonLayer>(Engine::JsonLayer("assets/json/testLayer.json", "TestLayer")));
	m_layerStack->push(std::make_shared<Engine::JsonLayer>(Engine::JsonLayer("assets/json/testLayer.json", "TestLayer2")));

	int i = 0;
	for (auto& it = m_layerStack->begin(); it != m_layerStack->end(); it++)
	{
		i++;
	}

	EXPECT_NE(i, 0);
}

TEST(DataTest, LoadLayer)
{
	auto& iterator = m_layerStack->begin();

	std::shared_ptr<Engine::Layer> layer;
	layer = *iterator;

	auto layerToTest = *dynamic_cast<Engine::JsonLayer*>(layer.get());

	EXPECT_NE(layerToTest.getGameObjects().empty(), true);
}

TEST(DataTest, PopLayer)
{
	auto& iterator = m_layerStack->begin();

	std::shared_ptr<Engine::Layer> layer;
	layer = *iterator;

	m_layerStack->pop(layer);

	int i = 0;
	for (auto& it = m_layerStack->begin(); it != m_layerStack->end(); it++)
	{
		i++;
	}

	EXPECT_NE(i, 2);
}

