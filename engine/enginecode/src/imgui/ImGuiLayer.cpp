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
		m_addComponentWindow = false;
		m_changeComponentWindow = false;
		m_removeGameObjectWindow = false;
		m_removeComponentWindow = false;
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
		if (ImGui::Button("Add component"))
		{
			m_addComponentWindow = true;
		}
		if (ImGui::Button("Change component"))
		{
			m_changeComponentWindow = true;
		}
		if (ImGui::Button("Remove component"))
		{
			m_removeComponentWindow = true;
		}
		if (ImGui::Button("Remove GameObject"))
		{
			m_removeGameObjectWindow = true;
		}
		ImGui::End();

		addGO();
		addComp();
		changeComp();
		removeComp();
		removeGO();

		ImGuiIO& io = ImGui::GetIO();
		glm::vec2 res = glm::vec2(800, 600);
		io.DisplaySize = ImVec2((float)res.x, (float)res.y);
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event & e)
	{
	}

	void ImGuiLayer::addGO()
	{
		if (m_gameObjectWindow)
		{
			ImGui::Begin("GameObject");

			static char goName[32] = "Default name";
			static glm::vec3 translation = glm::vec3(0.0f);
			static glm::vec3 rotation = glm::vec3(0.0f);
			static glm::vec3 scale = glm::vec3(1.0f);

			ImGui::InputText("Name", goName, IM_ARRAYSIZE(goName));
			ImGui::InputFloat3("Position", &translation.x, 2);
			ImGui::InputFloat3("Rotation", &rotation.x, 2);
			ImGui::InputFloat3("Scale", &scale.x, 2);

			if (ImGui::Button("Spawn"))
			{
				std::shared_ptr<PositionComponent> pos;
				pos = std::make_shared<PositionComponent>(PositionComponent(translation, rotation, scale));
				m_layer->getGameObjects()[goName] = std::make_shared<GameObject>(GameObject(goName));
				m_layer->getGameObjects()[goName]->addComponent(pos);
			}
			ImGui::SameLine(150);
			if (ImGui::Button("Close"))
				m_gameObjectWindow = false;

			ImGui::End();
		}
	}

	void ImGuiLayer::addComp()
	{
		if (m_addComponentWindow)
		{
			ImGui::Begin("Add component");

			static char goName[32] = "Default name";
			ImGui::InputText("Name", goName, IM_ARRAYSIZE(goName));
			std::string name;

			if (ImGui::CollapsingHeader("Material"))
			{
				static bool json;
				static bool assimp;
				static char modelName[32] = "none";
				static char shaderName[32] = "none";

				ImGui::Checkbox("Json", &json);
				ImGui::SameLine(100);
				ImGui::Checkbox("Assimp", &assimp);

				ImGui::InputText("Model", modelName, IM_ARRAYSIZE(modelName));
				ImGui::InputText("Shader", shaderName, IM_ARRAYSIZE(shaderName));

				if (ImGui::Button("Add"))
				{
					// check if component does not exist yet
						// set material up
						// add component
				}				
			}

			if (ImGui::CollapsingHeader("Texture"))
			{
				static char tex[32] = "none";
				ImGui::InputText("Diffuse texture", tex, IM_ARRAYSIZE(tex));

				if (ImGui::Button("Add"))
				{
					// check if component does not exist yet
						// get texture (possibly need to consider more textures?)
						// add component
				}
			}

			if (ImGui::CollapsingHeader("Velocity"))
			{
				static glm::vec3 linear = glm::vec3(0.0f);
				static glm::vec3 angular = glm::vec3(0.0f);

				ImGui::InputFloat3("Linear", &linear.x, 3);
				ImGui::InputFloat3("Angular", &angular.x, 3);

				if (ImGui::Button("Add"))
				{
					// check if component does not exist yet
						// add component
				}
			}

			if (ImGui::CollapsingHeader("Controller"))
			{
				static float moveSpeed = 0.0f;
				static float rotationSpeed = 0.0f;

				ImGui::InputFloat("Move speed", &moveSpeed, 0.01f, 1.0f, 2);
				ImGui::InputFloat("Rotation speed", &rotationSpeed, 0.01f, 1.0f, 2);

				if (ImGui::Button("Add"))
				{
					// check if component does not exist yet
						// add component
				}
			}

			if (ImGui::CollapsingHeader("Oscillate"))
			{
				// TODO state

				static float currTime;
				static float maxTime;
				static bool setTexture;

				ImGui::InputFloat("Current time", &currTime, 0.01f, 1.0f, 2);
				ImGui::InputFloat("Max time", &maxTime, 0.01f, 1.0f, 2);
				ImGui::Checkbox("Set texture?", &setTexture);

				if (ImGui::Button("Add"))
				{
					//check if component does not exist yet
						// add component
				}
			}
			
			ImGui::Spacing();
			if (ImGui::Button("Close"))
				m_addComponentWindow = false;
			ImGui::End();
		}
		
	}

	void ImGuiLayer::changeComp()
	{
		if (m_changeComponentWindow)
		{
			ImGui::Begin("Change component");

			static char goName[32] = "Default name";
			ImGui::InputText("Name", goName, IM_ARRAYSIZE(goName));
			std::string name;

			if (ImGui::Button("Close"))
				m_changeComponentWindow = false;
			ImGui::End();
		}
	}

	void ImGuiLayer::removeComp()
	{
		if (m_removeComponentWindow)
		{
			ImGui::Begin("Remove component");

			static char goName[32] = "Default name";
			ImGui::InputText("Name", goName, IM_ARRAYSIZE(goName));
			std::string name;

			if (ImGui::Button("Close"))
				m_removeComponentWindow = false;
			ImGui::End();
		}
	}

	void ImGuiLayer::removeGO()
	{
		if (m_removeGameObjectWindow)
		{
			ImGui::Begin("Remove GameObject");

			static char goName[32] = "Default name";

			ImGui::InputText("Name", goName, IM_ARRAYSIZE(goName));

			if (ImGui::Button("Remove"))
			{
				m_layer->getGameObjects().erase(goName);
			}
			ImGui::SameLine(150);
			if (ImGui::Button("Close"))
				m_removeGameObjectWindow = false;

			ImGui::End();
		}
	}
}




//std::shared_ptr<MaterialComponent> mat;
//std::shared_ptr<JsonModel> model = ResourceManagerInstance->getJsonModels().getAsset("JsonFCCube");
//std::string name = goName;
//ResourceManagerInstance->addVAO(name + "VAO");
//ResourceManagerInstance->addVBO(name + "VBO", model->vertices, sizeof(float) * model->verticesSize, model->shader->getBufferLayout());
//ResourceManagerInstance->addEBO(name + "EBO", model->indices, sizeof(unsigned int) * model->indicesSize);
//
//ResourceManagerInstance->getVAO().getAsset(name + "VAO")->
//setVertexBuffer(ResourceManagerInstance->getVBO().getAsset(name + "VBO"));
//ResourceManagerInstance->getVAO().getAsset(name + "VAO")->
//setIndexBuffer(ResourceManagerInstance->getEBO().getAsset(name + "EBO"));
//
//ResourceManagerInstance->addMaterial(name + "Mat",
//	ResourceManagerInstance->getShader().getAsset("FlatColorShader"),
//	ResourceManagerInstance->getVAO().getAsset(name + "VAO"));
//
//mat = std::make_shared<MaterialComponent>
//(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(name + "Mat")));
//m_layer->getGameObjects()[goName]->addComponent(mat);



