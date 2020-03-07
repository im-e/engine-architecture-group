/** \file Sandbox.cpp
*/

#include "Sandbox.h"

#include "systems/Log.h"
#include "data/json/JsonLayer.h"

namespace Sandbox
{
	Sandbox::Sandbox()
	{
		m_layerStack->push(std::make_shared<Engine::JsonLayer>(Engine::JsonLayer("assets/json/cubeLayer.json", "../levels/Default.json", "Game Layer")));
		m_layerStack->push(std::make_shared<Engine::JsonLayer>(Engine::JsonLayer("assets/json/uiLayer.json", "../levels/Default2D.json", "UI Layer")));
	}

	Sandbox::~Sandbox()
	{
	}
}

Engine::Application* Engine::startApplication()
{
	return new Sandbox::Sandbox();
}