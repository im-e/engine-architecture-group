/** \file Sandbox.cpp
*/

#include "Sandbox.h"

#include "systems/Log.h"
#include "data/json/JsonLayer.h"
#include "imgui/ImGuiLayer.h"

namespace Sandbox
{
	Sandbox::Sandbox()
	{
		m_layerStack->push(std::make_shared<Engine::JsonLayer>(Engine::JsonLayer("../levelEditor/assets/json/editorLayer.json", "../levels/Default.json", "Editor Layer")));
		m_layerStack->push(std::make_shared<Engine::ImGuiLayer>(Engine::ImGuiLayer("ImGui Layer", "../levelEditor/assets/json/components.json", (*m_layerStack->begin()))));
	}

	Sandbox::~Sandbox()
	{
	}
}

Engine::Application* Engine::startApplication()
{
	return new Sandbox::Sandbox();
}