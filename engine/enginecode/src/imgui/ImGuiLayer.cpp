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
		ImGui::SameLine(150);
		if (ImGui::Button("List"))
		{
			for (auto& go : m_layer->getGameObjects())
			{
				LogInfo("{0}", go.second->getName());
			}
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
				if (m_layer->getGameObjects()[goName] == NULL)
				{
					std::shared_ptr<PositionComponent> pos;
					pos = std::make_shared<PositionComponent>(PositionComponent(translation, rotation, scale));
					m_layer->getGameObjects()[goName] = std::make_shared<GameObject>(GameObject(goName));
					m_layer->getGameObjects()[goName]->addComponent(pos);
				}
				else
				{
					LogWarn("GameObject with name {0} already exists!", goName);
				}
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
			std::string name = goName;

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
					if (m_layer->getGameObjects()[name]->getComponent<MaterialComponent>() == nullptr)
					{
						// set material up
						if (json)
						{
							std::shared_ptr<MaterialComponent> mat;
							std::shared_ptr<JsonModel> model = ResourceManagerInstance->getJsonModels().getAsset(modelName);							

							ResourceManagerInstance->addVAO(name + "VAO");
							ResourceManagerInstance->addVBO(name + "VBO", model->vertices, sizeof(float) * model->verticesSize, model->shader->getBufferLayout());
							ResourceManagerInstance->addEBO(name + "EBO", model->indices, sizeof(unsigned int) * model->indicesSize);
							
							ResourceManagerInstance->getVAO().getAsset(name + "VAO")->
							setVertexBuffer(ResourceManagerInstance->getVBO().getAsset(name + "VBO"));
							ResourceManagerInstance->getVAO().getAsset(name + "VAO")->
							setIndexBuffer(ResourceManagerInstance->getEBO().getAsset(name + "EBO"));
							
							ResourceManagerInstance->addMaterial(name + "Mat",
								ResourceManagerInstance->getShader().getAsset(shaderName),
								ResourceManagerInstance->getVAO().getAsset(name + "VAO"));
							
							mat = std::make_shared<MaterialComponent>
							(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(name + "Mat")));
							m_layer->getGameObjects()[goName]->addComponent(mat);
						}
						else if (assimp)
						{
							std::shared_ptr<AssimpModel> assimpModel = ResourceManagerInstance->getAssimpModels().getAsset(modelName);

							for (int i = 0; i < assimpModel->m_meshes.size(); i++)
							{
								std::shared_ptr<MaterialComponent> mat;
								Mesh* mesh = &assimpModel->m_meshes.at(i);
								mesh->m_shader = ResourceManagerInstance->getShader().getAsset(shaderName);

								//set each mesh up
									// add VAO
								ResourceManagerInstance->addVAO(name + "VAO" + std::to_string(i));
								// add VBO
								ResourceManagerInstance->addVBO(name + "VBO" + std::to_string(i),
									(float*)&mesh->m_vertices.at(0),
									mesh->m_vertices.size() * sizeof(VertexData),
									mesh->m_shader->getBufferLayout());
								// add EBO
								ResourceManagerInstance->addEBO(name + "EBO" + std::to_string(i),
									&mesh->m_indices.at(0),
									mesh->m_indices.size() * sizeof(unsigned int));
								// link VBO to VAO
								ResourceManagerInstance->getVAO().getAsset(name + "VAO" + std::to_string(i))->
									setVertexBuffer(ResourceManagerInstance->getVBO().getAsset(name + "VBO" + std::to_string(i)));
								// link EBO to VAO
								ResourceManagerInstance->getVAO().getAsset(name + "VAO" + std::to_string(i))->
									setIndexBuffer(ResourceManagerInstance->getEBO().getAsset(name + "EBO" + std::to_string(i)));

								mesh->setupMesh(mesh->m_vertices.at(0), mesh->m_indices.at(0));

								// add material
								ResourceManagerInstance->addMaterial(name + "Mat" + std::to_string(i),
									mesh->m_shader,
									ResourceManagerInstance->getVAO().getAsset(name + "VAO" + std::to_string(i)));

								mat = std::make_shared<MaterialComponent>
									(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(name + "Mat" + std::to_string(i))));
								m_layer->getGameObjects()[goName]->addComponent(mat);
							}
						}
					}
				}				
			}

			if (ImGui::CollapsingHeader("Texture"))
			{
				static char tex[32] = "none";
				ImGui::InputText("Diffuse texture", tex, IM_ARRAYSIZE(tex));

				if (ImGui::Button("Add"))
				{
					if (m_layer->getGameObjects()[goName]->getComponent<TextureComponent>() == nullptr)
					{
						// get texture (possibly need to consider more textures?)
						std::shared_ptr<TextureComponent> texComp;
						texComp = std::make_shared<TextureComponent>
							(TextureComponent(ResourceManagerInstance->getTexture().getAsset(tex)->getSlot()));
						m_layer->getGameObjects()[goName]->addComponent(texComp);
					}
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
					if (m_layer->getGameObjects()[goName]->getComponent<VelocityComponent>() == nullptr)
					{
						std::shared_ptr<VelocityComponent> vel;
						vel = std::make_shared<VelocityComponent>(VelocityComponent(linear, angular));
						m_layer->getGameObjects()[goName]->addComponent(vel);
					}
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
					if (m_layer->getGameObjects()[goName]->getComponent<ControllerComponent>() == nullptr)
					{
						std::shared_ptr<ControllerComponent> ctr;
						ctr = std::make_shared<ControllerComponent>
							(ControllerComponent(moveSpeed, rotationSpeed));
						m_layer->getGameObjects()[goName]->addComponent(ctr);
					}
				}
			}

			if (ImGui::CollapsingHeader("Oscillate"))
			{
				// TODO state
				const char* items[3] = { "Stopped", "Down", "Up" };
				static const char* currentItem = items[0];

				OscillateComponent::State state = OscillateComponent::State::STOPPED;

				static float currTime;
				static float maxTime;
				static bool setTexture;

				if (ImGui::BeginCombo("State", currentItem))
				{
					for (int i = 0; i < IM_ARRAYSIZE(items); i++)
					{
						bool selected = (currentItem == items[i]);

						if (ImGui::Selectable(items[i], selected))
						{
							currentItem = items[i];
						}

						if (selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::InputFloat("Current time", &currTime, 0.01f, 1.0f, 2);
				ImGui::InputFloat("Max time", &maxTime, 0.01f, 1.0f, 2);
				ImGui::Checkbox("Set texture?", &setTexture);

				if (ImGui::Button("Add"))
				{
					//check if component does not exist yet
					if (m_layer->getGameObjects()[goName]->getComponent<OscillateComponent>() == nullptr)
					{
						std::shared_ptr<OscillateComponent> osc;
						VelocityComponent* vel = m_layer->getGameObjects()[goName]->getComponent<VelocityComponent>();

						std::string stateStr = currentItem;

						if (stateStr.compare("Stopped") == 0) state = OscillateComponent::State::STOPPED;
						if (stateStr.compare("Down") == 0) state = OscillateComponent::State::DOWN;
						if (stateStr.compare("Up") == 0) state = OscillateComponent::State::UP;

						osc = std::make_shared<OscillateComponent>
							(OscillateComponent(state, currTime, maxTime, setTexture, vel->getLinear()));
						m_layer->getGameObjects()[goName]->addComponent(osc);
					}
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








