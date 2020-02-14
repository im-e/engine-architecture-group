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

#include "platform/GLFW_KeyCodes.h"

namespace Engine
{
	void ImGuiLayer::onAttach()
	{
		m_gameObjectWindow = false;
		m_manageCompWindow = false;

		auto textures = ResourceManagerInstance->getTexture().getCollection();
		auto jsonModels = ResourceManagerInstance->getJsonModels().getCollection();
		auto assimpModels = ResourceManagerInstance->getAssimpModels().getCollection();
		auto shaders = ResourceManagerInstance->getShader().getCollection();

		for (auto& t : textures)
		{
			m_texturesNames.push_back(t.first);
		}

		for (auto& jM : jsonModels)
		{
			m_jsonModelNames.push_back(jM.first);
		}

		for (auto& aM : assimpModels)
		{
			m_assimpModelNames.push_back(aM.first);
		}

		for (auto& s : shaders)
		{
			m_shadersNames.push_back(s.first);
		}
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
		if (ImGui::Button("Manage components"))
		{
			m_manageCompWindow = true;
		}
		if (ImGui::CollapsingHeader("Save"))
		{
			static char fileName[32] = "Default";
			ImGui::InputText("", fileName, IM_ARRAYSIZE(fileName));
			ImGui::SameLine();
			ImGui::Text(".json");

			if (ImGui::Button("Save to file"))
			{
				std::string file = fileName;
				std::string path = "../levels/" + file + ".json";

				std::ofstream outputStream;
				outputStream.open(path);
				outputStream << "{ \"GameObject\": [ ";

				int i = 0;

				// save each gameobject
				for (auto& go : m_layer->getGameObjects())
				{
					auto comps = go.second->getComponents();
					std::string name = go.second->getName();

					outputStream << "{ ";
					outputStream << "\"name\": \"" + name + "\",";

					bool hasMat = false;
					int j = 0;

					for (auto& c : comps)
					{
						if (c->getType().hash_code() == typeid(MaterialComponent).hash_code() && hasMat == false)
						{
							hasMat = true;
							std::shared_ptr<MaterialComponent> comp = std::static_pointer_cast<MaterialComponent>(c);

							std::string type = comp->getTypeName();
							std::string model = comp->getModelName();
							std::string shader = comp->getShaderName();

							outputStream << "\"material\": { \"type\": \"" + type + "\", \"model\": \"" + model + "\", \"shader\": \"" + shader + "\"";
						}
						else if (c->getType().hash_code() == typeid(TextureComponent).hash_code())
						{
							std::shared_ptr<TextureComponent> comp = std::static_pointer_cast<TextureComponent>(c);
						}
						else if (c->getType().hash_code() == typeid(PositionComponent).hash_code())
						{
							std::shared_ptr<PositionComponent> comp = std::static_pointer_cast<PositionComponent>(c);
						}
						else if (c->getType().hash_code() == typeid(VelocityComponent).hash_code())
						{
							std::shared_ptr<VelocityComponent> comp = std::static_pointer_cast<VelocityComponent>(c);
						}
						else if (c->getType().hash_code() == typeid(ControllerComponent).hash_code())
						{
							std::shared_ptr<ControllerComponent> comp = std::static_pointer_cast<ControllerComponent>(c);
						}
						else if (c->getType().hash_code() == typeid(OscillateComponent).hash_code())
						{
							std::shared_ptr<OscillateComponent> comp = std::static_pointer_cast<OscillateComponent>(c);
						}


						if (j < comps.size() - 1)
						{
							outputStream << "},";
						}
						else
						{
							outputStream << "}";
						}
						j++;
						
					}
					if (i < m_layer->getGameObjects().size() - 1)
					{
						outputStream << "},";
					}
					else
					{
						outputStream << "}";
					}
					i++;
				}

				// write footer
				outputStream << "] }";
			}
		}
		ImGui::End();

		addGO();
		manageComponents();

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
			if (ImGui::Button("Delete"))
			{
				m_layer->getGameObjects().erase(goName);
			}			
			if (ImGui::Button("Close"))
				m_gameObjectWindow = false;

			ImGui::End();
		}
	}

	void ImGuiLayer::manageComponents()
	{
		if (m_manageCompWindow)
		{
			ImGui::Begin("Manage Components");

			static const char* goName = "";
			if (ImGui::BeginCombo("GameObject", goName))
			{
				for (auto& go : m_layer->getGameObjects())
				{
					bool selected = (goName == go.second->getName().c_str());

					if (ImGui::Selectable(go.second->getName().c_str(), selected))
					{
						goName = go.second->getName().c_str();
					}

					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			m_name = goName;

			if (ImGui::CollapsingHeader("Material"))
			{
				const char* types[2] = { "Json", "Assimp" };
				static const char* currentItem = types[0];
				if (ImGui::BeginCombo("Type", currentItem))
				{
					for (int i = 0; i < IM_ARRAYSIZE(types); i++)
					{
						bool selected = (currentItem == types[i]);

						if (ImGui::Selectable(types[i], selected))
						{
							currentItem = types[i];
						}

						if (selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				static const char* meshItem = "";
				if (currentItem == "Json")
				{
					if (ImGui::BeginCombo("Json Mesh", meshItem))
					{
						for (int i = 0; i < m_jsonModelNames.size(); i++)
						{
							bool selected = (meshItem == m_jsonModelNames[i]);

							if (ImGui::Selectable(m_jsonModelNames[i].c_str(), selected))
							{
								meshItem = m_jsonModelNames[i].c_str();
							}

							if (selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
				}
				else if (currentItem == "Assimp")
				{
					if (ImGui::BeginCombo("Assimp Mesh", meshItem))
					{
						for (int i = 0; i < m_assimpModelNames.size(); i++)
						{
							bool selected = (meshItem == m_assimpModelNames[i]);

							if (ImGui::Selectable(m_assimpModelNames[i].c_str(), selected))
							{
								meshItem = m_assimpModelNames[i].c_str();
							}

							if (selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
				}

				static const char* shaderName = "";
				if (ImGui::BeginCombo("Shader", shaderName))
				{
					for (int i = 0; i < m_shadersNames.size(); i++)
					{
						bool selected = (shaderName == m_shadersNames[i]);

						if (ImGui::Selectable(m_shadersNames[i].c_str(), selected))
						{
							shaderName = m_shadersNames[i].c_str();
						}

						if (selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("Add"))
				{
					// check if component does not exist yet
					if (m_layer->getGameObjects()[m_name]->getComponent<MaterialComponent>() == nullptr)
					{
						// set material up
						if (currentItem == "Json")
						{
							std::shared_ptr<MaterialComponent> mat;
							std::shared_ptr<JsonModel> model = ResourceManagerInstance->getJsonModels().getAsset(meshItem);

							ResourceManagerInstance->addVAO(m_name + "VAO");
							ResourceManagerInstance->addVBO(m_name + "VBO", model->vertices, sizeof(float) * model->verticesSize, model->shader->getBufferLayout());
							ResourceManagerInstance->addEBO(m_name + "EBO", model->indices, sizeof(unsigned int) * model->indicesSize);
							
							ResourceManagerInstance->getVAO().getAsset(m_name + "VAO")->
							setVertexBuffer(ResourceManagerInstance->getVBO().getAsset(m_name + "VBO"));
							ResourceManagerInstance->getVAO().getAsset(m_name + "VAO")->
							setIndexBuffer(ResourceManagerInstance->getEBO().getAsset(m_name + "EBO"));
							
							ResourceManagerInstance->addMaterial(m_name + "Mat",
								ResourceManagerInstance->getShader().getAsset(shaderName),
								ResourceManagerInstance->getVAO().getAsset(m_name + "VAO"));
							
							mat = std::make_shared<MaterialComponent>
								(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(m_name + "Mat")));
							mat->setTypeName("json");
							mat->setModelName(meshItem);
							mat->setShaderName(shaderName);
							m_layer->getGameObjects()[goName]->addComponent(mat);
						}
						else if (currentItem == "Assimp")
						{
							std::shared_ptr<AssimpModel> assimpModel = ResourceManagerInstance->getAssimpModels().getAsset(meshItem);
							
							for (int i = 0; i < assimpModel->m_meshes.size(); i++)
							{
								std::shared_ptr<MaterialComponent> mat;
								Mesh* mesh = &assimpModel->m_meshes.at(i);
								mesh->m_shader = ResourceManagerInstance->getShader().getAsset(shaderName);

								//set each mesh up
									// add VAO
								ResourceManagerInstance->addVAO(m_name + "VAO" + std::to_string(i));
								// add VBO
								ResourceManagerInstance->addVBO(m_name + "VBO" + std::to_string(i),
									(float*)&mesh->m_vertices.at(0),
									mesh->m_vertices.size() * sizeof(VertexData),
									mesh->m_shader->getBufferLayout());
								// add EBO
								ResourceManagerInstance->addEBO(m_name + "EBO" + std::to_string(i),
									&mesh->m_indices.at(0),
									mesh->m_indices.size() * sizeof(unsigned int));
								// link VBO to VAO
								ResourceManagerInstance->getVAO().getAsset(m_name + "VAO" + std::to_string(i))->
									setVertexBuffer(ResourceManagerInstance->getVBO().getAsset(m_name + "VBO" + std::to_string(i)));
								// link EBO to VAO
								ResourceManagerInstance->getVAO().getAsset(m_name + "VAO" + std::to_string(i))->
									setIndexBuffer(ResourceManagerInstance->getEBO().getAsset(m_name + "EBO" + std::to_string(i)));

								mesh->setupMesh(mesh->m_vertices.at(0), mesh->m_indices.at(0));

								// add material
								ResourceManagerInstance->addMaterial(m_name + "Mat" + std::to_string(i),
									mesh->m_shader,
									ResourceManagerInstance->getVAO().getAsset(m_name + "VAO" + std::to_string(i)));

								mat = std::make_shared<MaterialComponent>
									(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(m_name + "Mat" + std::to_string(i))));
								mat->setTypeName("assimp");
								mat->setModelName(meshItem);
								mat->setShaderName(shaderName);
								m_layer->getGameObjects()[goName]->addComponent(mat);
							}
						}
					}
				}
				ImGui::SameLine(100);
				if (ImGui::Button("Remove"))
				{
					if (m_layer->getGameObjects()[m_name]->getComponent<MaterialComponent>() != nullptr)
					{
						// set material up
						if (currentItem == "Json")
						{
							ResourceManagerInstance->getVAO().remove(m_name + "VAO");
							ResourceManagerInstance->getVBO().remove(m_name + "VBO");
							ResourceManagerInstance->getEBO().remove(m_name + "EBO");
							ResourceManagerInstance->getMaterial().remove(m_name + "Mat");

							m_layer->getGameObjects()[goName]->removeComponent(m_layer->getGameObjects()[goName]->getComponent<MaterialComponent>());
						}
						else if (currentItem == "Assimp")
						{
							std::shared_ptr<AssimpModel> assimpModel = ResourceManagerInstance->getAssimpModels().getAsset(meshItem);

							for (int i = 0; i < assimpModel->m_meshes.size(); i++)
							{
								ResourceManagerInstance->getVAO().remove(m_name + "VAO" + std::to_string(i));
								ResourceManagerInstance->getVBO().remove(m_name + "VBO");
								ResourceManagerInstance->getEBO().remove(m_name + "EBO" + std::to_string(i));
								ResourceManagerInstance->getMaterial().remove(m_name + "Mat" + std::to_string(i));

								m_layer->getGameObjects()[goName]->removeComponent(m_layer->getGameObjects()[goName]->getComponent<MaterialComponent>());
							}
						}
					}
					else
					{
						LogWarn("Component did not exist anyway!");
					}
				}
			}

			if (ImGui::CollapsingHeader("Texture"))
			{
				static const char* tex = "";

				if (ImGui::BeginCombo("Diffuse texture", tex))
				{
					for (int i = 0; i < m_texturesNames.size(); i++)
					{
						bool selected = (tex == m_texturesNames[i]);

						if (ImGui::Selectable(m_texturesNames[i].c_str(), selected))
						{
							tex = m_texturesNames[i].c_str();
						}

						if (selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("Add"))
				{
					if (m_layer->getGameObjects()[goName]->getComponent<TextureComponent>() == nullptr)
					{
						// TODO more textures
						std::shared_ptr<TextureComponent> texComp;
						texComp = std::make_shared<TextureComponent>
							(TextureComponent(ResourceManagerInstance->getTexture().getAsset(tex)->getSlot()));

						m_layer->getGameObjects()[goName]->addComponent(texComp);
					}
					else
					{
						LogWarn("Component already exists!");
					}
				}
				ImGui::SameLine(100);
				if (ImGui::Button("Change"))
				{
					auto comp = m_layer->getGameObjects()[goName]->getComponent<TextureComponent>();
					if (comp != nullptr)
					{
						comp->setTexture(ResourceManagerInstance->getTexture().getAsset(tex)->getSlot());
					}
					else
					{
						LogError("Can't change - add first!");
					}				
				}
				ImGui::SameLine(200);
				if (ImGui::Button("Remove"))
				{
					if (m_layer->getGameObjects()[goName]->getComponent<TextureComponent>() != nullptr)
					{
						auto comp = m_layer->getGameObjects()[goName]->getComponent<TextureComponent>();
						m_layer->getGameObjects()[goName]->removeComponent(comp);
					}
					else
					{
						LogWarn("Component did not exist anyway!");
					}
				}
			}

			if (ImGui::CollapsingHeader("Position"))
			{
				if (m_layer->getGameObjects().find(goName) != m_layer->getGameObjects().end())
				{
					static glm::vec3 pos = m_layer->getGameObjects()[goName]->getComponent<PositionComponent>()->getPosition();
					static glm::vec3 rot = m_layer->getGameObjects()[goName]->getComponent<PositionComponent>()->getRotation();
					static glm::vec3 scl = m_layer->getGameObjects()[goName]->getComponent<PositionComponent>()->getScale();

					ImGui::InputFloat3("Position", &pos.x, 2);
					ImGui::InputFloat3("Rotation", &rot.x, 2);
					ImGui::InputFloat3("Scale", &scl.x, 2);

					m_layer->getGameObjects()[goName]->getComponent<PositionComponent>()->setPosition(pos);
					m_layer->getGameObjects()[goName]->getComponent<PositionComponent>()->setRotation(rot);
					m_layer->getGameObjects()[goName]->getComponent<PositionComponent>()->setScale(scl);
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
				ImGui::SameLine(100);
				if (ImGui::Button("Change"))
				{
					auto comp = m_layer->getGameObjects()[goName]->getComponent<VelocityComponent>();
					if (comp)
					{
						comp->setLinear(linear);
						comp->setAngular(angular);
					}
					else
					{
						LogError("Can't change - add first!");
					}
				}
				ImGui::SameLine(200);
				if (ImGui::Button("Remove"))
				{
					if (m_layer->getGameObjects()[goName]->getComponent<VelocityComponent>())
					{
						auto comp = m_layer->getGameObjects()[goName]->getComponent<VelocityComponent>();
						m_layer->getGameObjects()[goName]->removeComponent(comp);
					}
					else
					{
						LogWarn("Component did not exist anyway!");
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
				ImGui::SameLine(100);
				if (ImGui::Button("Change"))
				{
					auto comp = m_layer->getGameObjects()[goName]->getComponent<ControllerComponent>();
					if (comp)
					{
						comp->setMoveSpeed(moveSpeed);
						comp->setRotationSpeeed(rotationSpeed);
					}
					else
					{
						LogError("Can't change - add first!");
					}
				}
				ImGui::SameLine(200);
				if (ImGui::Button("Remove"))
				{
					if (m_layer->getGameObjects()[goName]->getComponent<ControllerComponent>())
					{
						auto comp = m_layer->getGameObjects()[goName]->getComponent<ControllerComponent>();
						m_layer->getGameObjects()[goName]->removeComponent(comp);
					}
					else
					{
						LogWarn("Component did not exist anyway!");
					}
				}
			}

			if (ImGui::CollapsingHeader("Oscillate"))
			{
				std::vector<const char*> chars;
				chars.push_back("Stopped");
				chars.push_back("Down");
				chars.push_back("Up");
				static const char* currentItem = chars[0];

				OscillateComponent::State state = OscillateComponent::State::STOPPED;

				static float currTime;
				static float maxTime;
				static bool setTexture;

				if (ImGui::BeginCombo("State", currentItem))
				{
					for (int i = 0; i < chars.size(); i++)
					{
						bool selected = (currentItem == chars[i]);

						if (ImGui::Selectable(chars[i], selected))
						{
							currentItem = chars[i];
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
						std::shared_ptr<VelocityComponent> vel = m_layer->getGameObjects()[goName]->getComponent<VelocityComponent>();

						std::string stateStr = currentItem;

						if (stateStr.compare("Stopped") == 0) state = OscillateComponent::State::STOPPED;
						if (stateStr.compare("Down") == 0) state = OscillateComponent::State::DOWN;
						if (stateStr.compare("Up") == 0) state = OscillateComponent::State::UP;

						osc = std::make_shared<OscillateComponent>
							(OscillateComponent(state, currTime, maxTime, setTexture, vel->getLinear()));
						m_layer->getGameObjects()[goName]->addComponent(osc);
					}
				}
				ImGui::SameLine(100);
				if (ImGui::Button("Change"))
				{
					LogWarn("Oscillate component will be deprecated - can't change!");
				}
				ImGui::SameLine(200);
				if (ImGui::Button("Remove"))
				{
					if (m_layer->getGameObjects()[goName]->getComponent<OscillateComponent>())
					{
						auto comp = m_layer->getGameObjects()[goName]->getComponent<OscillateComponent>();
						m_layer->getGameObjects()[goName]->removeComponent(comp);
					}
					else
					{
						LogWarn("Component did not exist anyway!");
					}
				}
			}
			
			ImGui::Spacing();
			if (ImGui::Button("Close"))
				m_manageCompWindow = false;
			ImGui::End();
		}
		
	}
}








