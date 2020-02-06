/** \file Editor.cpp
*/

#include "Editor.h"

#include "systems/Log.h"
#include "data/json/JsonLayer.h"

namespace Editor
{
	LevelEditor::LevelEditor()
	{
		m_layerStack->push(std::make_shared<Engine::JsonLayer>(Engine::JsonLayer("assets/json/editorLayer.json", "Editor Layer")));
		// ImGui layer
	}

	LevelEditor::~LevelEditor()
	{
	}
}

Engine::Application* Engine::startApplication()
{
	return new Editor::LevelEditor();
}



