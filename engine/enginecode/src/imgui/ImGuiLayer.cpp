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
#include "components/RigidBodyComponent.h"
#include "components/ColliderComponent.h"
#include "components/BoxColliderComponent.h"
#include "components/CapsuleColliderComponent.h"
#include "components/SphereColliderComponent.h"

#include "platform/GLFW_KeyCodes.h"
#include "data/json/JsonLoader.h"

namespace Engine
{
	bool ImGuiLayer::m_is2D;

	void ImGuiLayer::onAttach()
	{
		m_gameObjectWindow = false;
		m_manageCompWindow = false;
		m_managePPWindow = false;

		auto textures = ResourceManagerInstance->getTexture().getCollection();
		auto jsonModels = ResourceManagerInstance->getJsonModels().getCollection();
		auto assimpModels = ResourceManagerInstance->getAssimpModels().getCollection();
		auto shaders = ResourceManagerInstance->getShader().getCollection();
		auto sounds = ResourceManagerInstance->getSound().getCollection();

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

		for (auto& s : sounds)
		{
			m_soundNames.push_back(s.first);
		}

		m_pathTypes.push_back("Single");
		m_pathTypes.push_back("Constant");
		m_pathTypes.push_back("Reversing");

		Engine::JsonLoader::loadImGui(m_filepath, *this);
	}

	void ImGuiLayer::onDetach()
	{
	}

	void ImGuiLayer::onUpdate(float timestep)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Editor window");
		if (ImGui::Button("Add GameObject"))
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
		if (ImGui::Button("Post Processing"))
		{
			m_managePPWindow = true;
		}

		ImGui::SameLine(150);
		ImGui::Checkbox("Manage 2D", &m_is2D);
		if (m_is2D)
		{
			m_layer = static_cast<JsonLayer*>(Application::getInstance().getLayerStack()->getLayers()[1].get());
		}
		else
		{
			m_layer = static_cast<JsonLayer*>(Application::getInstance().getLayerStack()->getLayers()[0].get());
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

					outputStream << "{ \"name\": \"" + name + "\",";

					bool hasMat = false;					
					bool floatingBracket = false;
					int j = 0;

					for (auto& c : comps)
					{
						auto typeHash = c->getType().hash_code();
						bool isMat = typeHash == typeid(MaterialComponent).hash_code();
						bool isTexture = typeHash == typeid(TextureComponent).hash_code();
						bool isColour = typeHash == typeid(ColourComponent).hash_code();

						if (floatingBracket && !isMat && !isTexture && !isColour)
						{
							outputStream << "},";
							floatingBracket = false;
						}

						if (c->getType().hash_code() == typeid(TextComponent).hash_code())
						{
							std::shared_ptr<TextComponent> comp = std::static_pointer_cast<TextComponent>(c);

							std::string text = comp->getLabel()->getText();
							std::string font = comp->getLabel()->getFont();
							unsigned int charSize = comp->getLabel()->getCharSize();

							std::shared_ptr<ColourComponent> cComp = go.second->getComponent<ColourComponent>();

							float r = cComp->getRGB().x;
							float g = cComp->getRGB().y;
							float b = cComp->getRGB().z;

							outputStream << "\"material\": { \"text\": \"" << text << "\", \"font\": \"" << font << "\", \"charSize\": " << std::to_string(charSize) << ", \"colour\": { \"r\": " << std::to_string(r) << ", \"g\": " << std::to_string(g) << ", \"b\": " << std::to_string(b) << "}";
						}
						else if (c->getType().hash_code() == typeid(MaterialComponent).hash_code() && hasMat == false)
						{
							hasMat = true;

							if (!m_is2D)
							{
								std::shared_ptr<MaterialComponent> comp = std::static_pointer_cast<MaterialComponent>(c);

								std::string type = comp->getTypeName();
								std::string model = comp->getModelName();
								std::string shader = comp->getShaderName();

								outputStream << "\"material\": { \"type\": \"" + type + "\", \"model\": \"" + model + "\", \"shader\": \"" + shader + "\"";
							}		
						}
						else if (c->getType().hash_code() == typeid(TextureComponent).hash_code())
						{
							if (!m_is2D)
							{
								std::shared_ptr<TextureComponent> comp = std::static_pointer_cast<TextureComponent>(c);

								std::string diffuse = comp->getDiffName();
								std::string normal = comp->getNormalName();
								std::string parallax = comp->getParallaxName();
								std::string spec = comp->getSpecularName();

								outputStream << "\"texture\": { \"diffuse\": \"" + diffuse + "\", \"specular\": \"" + spec + "\", \"normal\": \"" + normal + "\", \"parallax\": \"" + parallax + "\"";
							}					
						}
						else if (c->getType().hash_code() == typeid(PositionComponent).hash_code())
						{
							std::shared_ptr<PositionComponent> comp = std::static_pointer_cast<PositionComponent>(c);

							glm::vec3 pos(comp->getInitialPosition().x, comp->getInitialPosition().y, comp->getInitialPosition().z);
							glm::vec3 rot(comp->getInitialRotation().x, comp->getInitialRotation().y, comp->getInitialRotation().z);
							glm::vec3 scl(comp->getInitialScale().x, comp->getInitialScale().y, comp->getInitialScale().z);

							outputStream << "\"position\": { \"translation\": { \"x\": " + std::to_string(pos.x) + ", \"y\": " + std::to_string(pos.y) + ",\"z\": " + std::to_string(pos.z) + "},"
								+ "\"rotation\": { \"x\": " + std::to_string(rot.x) + ", \"y\": " + std::to_string(rot.y) + ",\"z\": " + std::to_string(rot.z) + "},"
								+ "\"scale\": { \"x\": " + std::to_string(scl.x) + ", \"y\": " + std::to_string(scl.y) + ",\"z\": " + std::to_string(scl.z) + "}";
						}
						else if (c->getType().hash_code() == typeid(VelocityComponent).hash_code())
						{
							std::shared_ptr<VelocityComponent> comp = std::static_pointer_cast<VelocityComponent>(c);

							glm::vec3 linear(comp->getLinear().x, comp->getLinear().y, comp->getLinear().z);
							glm::vec3 angular(comp->getAngular().x, comp->getAngular().y, comp->getAngular().z);

							outputStream << "\"velocity\": { \"linear\": { \"x\": " + std::to_string(linear.x) + ", \"y\": " + std::to_string(linear.y) + ",\"z\": " + std::to_string(linear.z) + "},"
								+ "\"angular\": { \"x\": " + std::to_string(angular.x) + ", \"y\": " + std::to_string(angular.y) + ",\"z\": " + std::to_string(angular.z) + "}";
						}
						else if (c->getType().hash_code() == typeid(ControllerComponent).hash_code())
						{
							std::shared_ptr<ControllerComponent> comp = std::static_pointer_cast<ControllerComponent>(c);

							float move = comp->getMoveSpeed();
							float rotation = comp->getRotationSpeed();

							outputStream << "\"controller\": { \"moveSpeed\": " + std::to_string(move) + ",\"rotationSpeed\": " + std::to_string(rotation);
						}
						else if (c->getType().hash_code() == typeid(AIComponent).hash_code())
						{
							std::shared_ptr<AIComponent> comp = std::static_pointer_cast<AIComponent>(c);

							float stopDist = comp->getStopDist();
							std::string aiType = comp->getAiType();
							std::string scriptName = comp->getScriptName();
							std::string pathType = comp->getPathTypeName();
							std::vector<glm::vec3> waypoints = comp->getPathWaypoints();

							outputStream << "\"AI\": { \"stopDist\": " + std::to_string(stopDist) + ", \"aiType\": \"" + aiType + "\", \"script\": \"" + scriptName + "\", \"pathType\": \"" + pathType + "\", \"waypoints\": [";

							for (int i = 0; i < waypoints.size(); i++)
							{
								outputStream << "{ \"x\": " + std::to_string(waypoints[i].x) + ", \"y\": " + std::to_string(waypoints[i].y) + ",\"z\": " + std::to_string(waypoints[i].z);

								if (i == waypoints.size() - 1)
								{
									outputStream << "}";
								}
								else
								{
									outputStream << "},";
								}
							}

							outputStream << "]";
						}
						else if (c->getType().hash_code() == typeid(SoundComponent).hash_code())
						{
							std::shared_ptr<SoundComponent> comp = std::static_pointer_cast<SoundComponent>(c);

							std::string soundName = comp->getSoundName();
							std::string audioType = comp->getAudioType();
							float volume = comp->getVolume();
							bool playOnAwake = comp->playOnAwake();

							outputStream << "\"sound\": { \"soundName\": \"" + soundName + "\", \"audioType\": \"" + audioType + "\", \"volume\": " + std::to_string(volume) + ", \"playOnAwake\": " + std::to_string(playOnAwake);
						}
						else if (c->getType().hash_code() == typeid(RigidBodyComponent).hash_code())
						{
							std::shared_ptr<RigidBodyComponent> comp = std::static_pointer_cast<RigidBodyComponent>(c);

							rp3d::BodyType rbType = comp->getBody()->getType();
							bool grav = comp->getGravity();

							std::string type = "";

							switch (rbType)
							{
							case rp3d::BodyType::STATIC:
								type = "Static";
								break;
							case rp3d::BodyType::KINEMATIC:
								type = "Kinematic";
								break;
							case rp3d::BodyType::DYNAMIC:
								type = "Dynamic";
								break;
							}

							outputStream << "\"rb\": { \"type\": \"" + type + "\", \"gravity\": " + std::to_string(grav);

						}
						else if (c->getType().hash_code() == typeid(BoxColliderComponent).hash_code())
						{
							std::shared_ptr<BoxColliderComponent> comp = std::static_pointer_cast<BoxColliderComponent>(c);

							std::string colliderType = "Box";
							float sizeX = comp->getSize().x;
							float sizeY = comp->getSize().y;
							float sizeZ = comp->getSize().z;

							outputStream << "\"collider\": { \"type\": \"" + colliderType + "\", \"sizeX\": " + std::to_string(sizeX) + ", \"sizeY\": " + std::to_string(sizeY) + ", \"sizeZ\": " + std::to_string(sizeZ);
						}
						else if (c->getType().hash_code() == typeid(CapsuleColliderComponent).hash_code())
						{
							std::shared_ptr<CapsuleColliderComponent> comp = std::static_pointer_cast<CapsuleColliderComponent>(c);

							std::string colliderType = "Capsule";
							float radius = comp->getRadius();
							float height = comp->getHeight();

							outputStream << "\"collider\": { \"type\": \"" + colliderType + "\", \"radius\": " + std::to_string(radius) + ", \"height\": " + std::to_string(height);
						}
						else if (c->getType().hash_code() == typeid(SphereColliderComponent).hash_code())
						{
							std::shared_ptr<SphereColliderComponent> comp = std::static_pointer_cast<SphereColliderComponent>(c);

							std::string colliderType = "Sphere";
							float radius = comp->getRadius();

							outputStream << "\"collider\": { \"type\": \"" + colliderType + "\", \"radius\": " + std::to_string(radius);
						}

						if (j < comps.size() - 1)
						{
							if (!m_is2D)
							{
								outputStream << "},";
							}
							else
							{
								bool x = c->getType().hash_code() == typeid(MaterialComponent).hash_code();
								bool y = c->getType().hash_code() == typeid(TextureComponent).hash_code();
								bool z = c->getType().hash_code() == typeid(ColourComponent).hash_code();

								if (x == false && y == false && z == false)
								{
									floatingBracket = true;
								}								
							}
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
		managePPRenderer();

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

			for (auto& fn : m_functions)
			{
				fn(m_layer);
			}						
			
			ImGui::Spacing();
			if (ImGui::Button("Close"))
				m_manageCompWindow = false;
			ImGui::End();
		}
		
	}

	void ImGuiLayer::managePPRenderer()
	{
		if (m_managePPWindow)
		{
			if (ImGui::CollapsingHeader("Post Processing"))
			{
				static const char* PPRShader = "";
				static int PPRShaderIndex = 2;

				if (ImGui::BeginCombo("Shaders", PPRShader))
				{
					for (int i = 0; i < m_shadersNames.size(); i++)
					{
						bool selected = (PPRShader == m_shadersNames[i]);

						if (ImGui::Selectable(m_shadersNames[i].c_str(), selected))
						{
							PPRShader = m_shadersNames[i].c_str();
							PPRShaderIndex = i;
						}

						if (selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				if (ImGui::Button("Add"))
				{
					auto renderer = m_layer->getRenderer();
					auto PPRender = std::static_pointer_cast<Engine::PPRenderer>(renderer);
					PPRender->setPPIndex(PPRShaderIndex);
					LogWarn(PPRShaderIndex);
				}
			}
			ImGui::Spacing();
			if (ImGui::Button("Close"))
				m_managePPWindow = false;
		}
	}

	void ImGuiLayer::addImGuiFunction(std::function<void(JsonLayer*)> func)
	{
		m_functions.push_back(func);
	}
	
	std::vector<std::function<void(JsonLayer*)>>& ImGuiLayer::getFunctions()
	{
		return m_functions;
	}

	std::vector<std::string>& ImGuiLayer::getShaderNames()
	{
		return m_shadersNames;
	}

	std::vector<std::string>& ImGuiLayer::getJsonModelsNames()
	{
		return m_jsonModelNames;
	}

	std::vector<std::string>& ImGuiLayer::getAssimpModelsNames()
	{
		return m_assimpModelNames;
	}

	std::vector<std::string>& ImGuiLayer::getTexturesNames()
	{
		return m_texturesNames;
	}

	std::vector<std::string>& ImGuiLayer::getPathTypes()
	{
		return m_pathTypes;
	}

	std::vector<std::string>& ImGuiLayer::getSoundNames()
	{
		return m_soundNames;
	}

	JsonLayer * ImGuiLayer::getJsonLayer()
	{
		return m_layer;
	}

	std::string & ImGuiLayer::getGOName()
	{
		return m_name;
	}
}