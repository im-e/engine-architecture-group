/** \file Editor.cpp
*/

#include "Editor.h"

#include "systems/Log.h"
#include "data/json/JsonLayer.h"
#include "imgui/ImGuiLayer.h"

namespace Editor
{
	LevelEditor::LevelEditor()
	{
		m_layerStack->push(std::make_shared<Engine::JsonLayer>(Engine::JsonLayer("assets/json/editorLayer.json", "../levels/Default.json", "Editor Layer")));
		m_layerStack->push(std::make_shared<Engine::JsonLayer>(Engine::JsonLayer("assets/json/uiLayer.json", "../levels/Default2D.json", "UI Layer")));
		m_layerStack->push(std::make_shared<Engine::ImGuiLayer>(Engine::ImGuiLayer("ImGui Layer", "assets/json/components.json", (*m_layerStack->begin()))));
	}

	LevelEditor::~LevelEditor()
	{
	}
}

Engine::Application* Engine::startApplication()
{
	return new Editor::LevelEditor();
}



