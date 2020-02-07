#include "engine_pch.h"
#include "imgui/ImGuiLayer.h"

#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

#include "components/GameObject.h"
#include "components/MaterialComponent.h"
#include "components/PositionComponent.h"
#include "components/VelocityComponent.h"
#include "components/OscillateComponent.h"
#include "components/TextureComponent.h"
#include "components/ControllerComponent.h"

namespace Engine
{
	void ImGuiLayer::onAttach()
	{
		m_gameObjectWindow = false;
	}

	void ImGuiLayer::onDetach()
	{
	}

	void ImGuiLayer::onUpdate(float timestep)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// https://eliasdaler.github.io/using-imgui-with-sfml-pt2/
		ImGui::Begin("Editor window");
		if(ImGui::Button("Add GameObject"))
		{
			m_gameObjectWindow = true;
		}
		ImGui::End();

		if (m_gameObjectWindow)
		{
			ImGui::Begin("GameObject");

			static char goName[32] = "Default name";
			ImGui::InputText("Name", goName, IM_ARRAYSIZE(goName));

			if (ImGui::Button("Spawn")) LogInfo("{0}", goName);
			ImGui::SameLine(150);
			if (ImGui::Button("Close")) m_gameObjectWindow = false;

			ImGui::End();
		}

		ImGuiIO& io = ImGui::GetIO();
		glm::vec2 res = glm::vec2(800, 600);
		io.DisplaySize = ImVec2((float)res.x, (float)res.y);
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event & e)
	{
	}
}








//m_layer->getGameObjects().push_back(std::make_shared<GameObject>(GameObject("Spawned cube")));
//auto go = m_layer->getGameObjects().back();
//std::shared_ptr<JsonModel> model = ResourceManagerInstance->getJsonModels().getAsset("JsonFCCube");
//
//ResourceManagerInstance->addVAO("Spawned cubeVAO");
//ResourceManagerInstance->addVBO("Spawned cubeVBO", model->vertices, sizeof(float) * model->verticesSize, model->shader->getBufferLayout());
//ResourceManagerInstance->addEBO("Spawned cubeEBO", model->indices, sizeof(unsigned int) * model->indicesSize);
//
//ResourceManagerInstance->getVAO().getAsset("Spawned cubeVAO")->
//setVertexBuffer(ResourceManagerInstance->getVBO().getAsset("Spawned cubeVBO"));
//ResourceManagerInstance->getVAO().getAsset("Spawned cubeVAO")->
//setIndexBuffer(ResourceManagerInstance->getEBO().getAsset("Spawned cubeEBO"));
//
//ResourceManagerInstance->addMaterial("Spawned cubeMat",
//	ResourceManagerInstance->getShader().getAsset("FlatColorShader"),
//	ResourceManagerInstance->getVAO().getAsset("Spawned cubeVAO"));
//
//m_layer->getMaterials().push_back(std::make_shared<MaterialComponent>
//(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset("Spawned cubeMat"))));
//go->addComponent(m_layer->getMaterials().back());
//
//m_layer->getPositions().push_back(std::make_shared<PositionComponent>
//(PositionComponent(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f))));
//go->addComponent(m_layer->getPositions().back());