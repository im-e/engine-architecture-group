#pragma once

/*! \file JsonLoader.h
\brief Implementation of a loader for json files
*/

#include <json.hpp>
#include <fstream>

#include "JsonLog.h"
#include "JsonLayer.h"
#include "JsonModelLoader.h" //TODO replace with assimp

namespace Engine
{
	/*! \class JsonLoader
	\brief Allows to create layers from json files
	*/
	class JsonLoader
	{
	private:
	public:
		//! Loads and populates layers defined in json files \param filepath path to the json file \param layer layer to be loaded
		static void loadJson(const std::string& filepath, JsonLayer& layer)
		{
#ifdef NG_DEBUG
			Engine::JsonLog::loadLog(filepath);
#endif
			std::fstream file(filepath, std::ios::in);

			if (!file.is_open())
			{
				LogError("Could not open {0}", filepath);
				return;
			}

			nlohmann::json jsonFile;
			file >> jsonFile;

			if (jsonFile.count("Asyncload") > 0)
			{
				if (jsonFile["Asyncload"].count("shaders") > 0)
				{
					for (auto& fnFilepath : jsonFile["Asyncload"]["shaders"])
					{
						std::string name = fnFilepath["name"].get<std::string>();
						std::string path = fnFilepath["filepath"].get<std::string>();
						ResourceManagerInstance->addShader(name, path);
					}
				}
				if (jsonFile["Asyncload"].count("textures") > 0)
				{
					for (auto& fnFilepath : jsonFile["Asyncload"]["textures"])
					{
						std::string name = fnFilepath["name"].get<std::string>();
						std::string path = fnFilepath["filepath"].get<std::string>();
						ResourceManagerInstance->addTexture(name, path);
					}
				}
				if (jsonFile["Asyncload"].count("fonts") > 0)
				{
					for (auto& fnFilepath : jsonFile["Asyncload"]["shaders"])
					{
						//TODO add fonts
					}
				}
			}

			if (jsonFile.count("MemoryInfo") > 0)
			{
				if (jsonFile["MemoryInfo"].count("gameObjects") > 0)
					layer.getGameObjects().resize(jsonFile["MemoryInfo"]["gameObjects"].get<int>());

				if (jsonFile["MemoryInfo"].count("materials") > 0)
					layer.getMaterials().resize(jsonFile["MemoryInfo"]["materials"].get<int>());

				if (jsonFile["MemoryInfo"].count("position") > 0)
					layer.getPositions().resize(jsonFile["MemoryInfo"]["position"].get<int>());

				if (jsonFile["MemoryInfo"].count("velocity") > 0)
					layer.getVelocities().resize(jsonFile["MemoryInfo"]["velocity"].get<int>());

				if (jsonFile["MemoryInfo"].count("oscillates") > 0)
					layer.getOscillates().resize(jsonFile["MemoryInfo"]["oscillates"].get<int>());

				if (jsonFile["MemoryInfo"].count("textures") > 0)
					layer.getTextures().resize(jsonFile["MemoryInfo"]["textures"].get<int>());

				if (jsonFile["MemoryInfo"].count("controllers") > 0)
					layer.getControllers().resize(jsonFile["MemoryInfo"]["controllers"].get<int>());
			}

			if (jsonFile.count("Camera") > 0)
			{
				std::string type = jsonFile["Camera"]["type"].get<std::string>();

				if (type.compare("Fps3D") == 0)
				{
					float translationSpeed = jsonFile["Camera"]["translationSpeed"].get<float>();
					float rotationSpeed = jsonFile["Camera"]["rotationSpeed"].get<float>();

					layer.getCamera().reset(Engine::CameraController::create3DController(translationSpeed, rotationSpeed));

					float fov = jsonFile["Camera"]["fov"].get<float>();
					float aspectRatio = jsonFile["Camera"]["aspectRatio"].get<float>();
					float nearClip = jsonFile["Camera"]["nearClip"].get<float>();
					float farClip = jsonFile["Camera"]["farClip"].get<float>();
					
					layer.getCamera()->init(fov, aspectRatio, nearClip, farClip);
				}

				else if (type.compare("Ortho2D") == 0)
				{
					float translationSpeed = jsonFile["Camera"]["translationSpeed"].get<float>();
					layer.getCamera().reset(CameraController::create2DController(translationSpeed));

					float top = jsonFile["Camera"]["top"].get<float>();
					float left = jsonFile["Camera"]["left"].get<float>();
					float width = jsonFile["Camera"]["width"].get<float>();
					float height = jsonFile["Camera"]["height"].get<float>();
					
					layer.getCamera()->init(left, width, height, top);
				}
			}

			if (jsonFile.count("Renderer") > 0)
			{
				std::string type = jsonFile["Renderer"]["type"].get<std::string>();
				if (type.compare("2D") == 0)
					layer.getRenderer().reset(Engine::Renderer::create2D());

				if (type.compare("3D") == 0)
					layer.getRenderer().reset(Engine::Renderer::create3D());
			}

			if (jsonFile.count("GameObject") > 0)
			{
				int goIndex = 0;
				int materialsIndex = 0;
				int positionsIndex = 0;
				int velocitiesIndex = 0;
				int oscillatesIndex = 0;
				int textureIndex = 0;
				int controllerIndex = 0;

				for (auto& go : jsonFile["GameObject"])
				{
					std::string goName = "Name";
					if (go.count("name") > 0)
					{
						goName = go["name"].get<std::string>();
					}
					layer.getGameObjects().at(goIndex) = std::make_shared<GameObject>(GameObject(goName));
					auto gameObject = layer.getGameObjects().at(goIndex);
					goIndex++;

					if (go.count("material") > 0)
					{
						if (go["material"].count("model") > 0)
						{
							JsonModel model;

							JsonModelLoader::loadModel(go["material"]["model"].get<std::string>(), model);
							ResourceManagerInstance->addVAO(goName+"VAO");
							ResourceManagerInstance->addVBO(goName+"VBO", model.vertices, sizeof(float) * model.verticesSize, model.shader->getBufferLayout());
							ResourceManagerInstance->addEBO(goName+"EBO", model.indices, sizeof(unsigned int) * model.indicesSize);

							ResourceManagerInstance->getVAO().getAsset(goName + "VAO")->
								setVertexBuffer(ResourceManagerInstance->getVBO().getAsset(goName + "VBO"));
							ResourceManagerInstance->getVAO().getAsset(goName + "VAO")->
								setIndexBuffer(ResourceManagerInstance->getEBO().getAsset(goName + "EBO"));

							ResourceManagerInstance->addMaterial(goName+"Mat",
									ResourceManagerInstance->getShader().getAsset(go["material"]["shader"].get<std::string>()),
									ResourceManagerInstance->getVAO().getAsset(goName + "VAO"));

							layer.getMaterials().at(materialsIndex) = std::make_shared<MaterialComponent>
								(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(goName+"Mat")));
							gameObject->addComponent(layer.getMaterials().at(materialsIndex));
							materialsIndex++;

							if (go.count("texture") > 0)
							{
								std::string texName = go["texture"]["name"].get<std::string>();

								layer.getTextures().at(textureIndex) = std::make_shared<TextureComponent>
									(TextureComponent(ResourceManagerInstance->getTexture().getAsset(texName)->getSlot()));
								gameObject->addComponent(layer.getTextures().at(textureIndex));
								textureIndex++;
							}
						}
						//TODO add text
					}
					if (go.count("position") > 0)
					{
						glm::vec3 translation(go["position"]["translation"]["x"].get<float>(), go["position"]["translation"]["y"].get<float>(), go["position"]["translation"]["z"].get<float>());
						glm::vec3 rotation(go["position"]["rotation"]["x"].get<float>(), go["position"]["rotation"]["y"].get<float>(), go["position"]["rotation"]["z"].get<float>());
						glm::vec3 scale(go["position"]["scale"]["x"].get<float>(), go["position"]["scale"]["y"].get<float>(), go["position"]["scale"]["z"].get<float>());

						layer.getPositions().at(positionsIndex) = std::make_shared<PositionComponent>
							(PositionComponent(translation, rotation, scale));
						gameObject->addComponent(layer.getPositions().at(positionsIndex));
						positionsIndex++;
					}

					if (go.count("velocity") > 0) 
					{
						glm::vec3 linear(go["velocity"]["linear"]["x"].get<float>(), go["velocity"]["linear"]["y"].get<float>(), go["velocity"]["linear"]["z"].get<float>());
						glm::vec3 angular(go["velocity"]["angular"]["x"].get<float>(), go["velocity"]["angular"]["y"].get<float>(), go["velocity"]["angular"]["z"].get<float>());

						layer.getVelocities().at(velocitiesIndex) = std::make_shared<VelocityComponent>
							(VelocityComponent(linear, angular));
						gameObject->addComponent(layer.getVelocities().at(velocitiesIndex));
						velocitiesIndex++;

						if (go.count("oscillate") > 0)
						{
							OscillateComponent::State state = OscillateComponent::State::DOWN;

							auto stateStr = go["oscillate"]["state"].get<std::string>();
							if (stateStr.compare("DOWN") == 0) state = OscillateComponent::State::DOWN;
							if (stateStr.compare("STOPPED") == 0) state = OscillateComponent::State::STOPPED;
							if (stateStr.compare("UP") == 0) state = OscillateComponent::State::UP;

							float currTime = go["oscillate"]["current time"].get<float>();
							float maxTime = go["oscillate"]["max time"].get<float>();
							bool setT = go["oscillate"]["set texture"].get<bool>();

							layer.getOscillates().at(oscillatesIndex) = std::make_shared<OscillateComponent>
								(OscillateComponent(state, currTime, maxTime, setT, layer.getVelocities().at(velocitiesIndex-1)->getLinear()));
							gameObject->addComponent(layer.getOscillates().at(oscillatesIndex));
							oscillatesIndex++;
						}

						if (go.count("controller") > 0)
						{
							float moveSpeed = go["controller"]["moveSpeed"].get<float>();
							float rotationSpeed = go["controller"]["rotationSpeed"].get<float>();

							layer.getControllers().at(controllerIndex) = std::make_shared<ControllerComponent>
								(ControllerComponent(moveSpeed, rotationSpeed));
							gameObject->addComponent(layer.getControllers().at(controllerIndex));
							controllerIndex++;
						}
					}				
				}
			}

			if (jsonFile.count("UBO") > 0)
			{
				for (auto& ubo : jsonFile["UBO"])
				{
					std::string uboName = ubo["name"].get<std::string>();
					UniformLayout uboLayout = {};

					if (ubo.count("layout") > 0)
					{
						for (auto& lay : ubo["layout"])
						{
							std::string SDT = lay.get<std::string>();
							if (SDT.compare("Vec3") == 0) { uboLayout.addElement(ShaderDataType::Float3); }
							if (SDT.compare("Mat4") == 0) { uboLayout.addElement(ShaderDataType::Mat4); }
							//TODO add all shader data types
						}
					}

					ResourceManagerInstance->addUBO(uboName, uboLayout.getStride(), uboLayout);

					if (ubo.count("shaders") > 0)
					{
						for (auto& shader : ubo["shaders"])
						{
							if (shader.count("name") > 0 && shader.count("block") > 0)
							{
								std::string shaderName = shader["name"].get<std::string>();
								std::string blockName = shader["block"].get<std::string>();

								ResourceManagerInstance->getUBO().getAsset(uboName)->
									attachShaderBlock(ResourceManagerInstance->getShader().getAsset(shaderName), blockName);
							}
						}
					}

					std::vector<void*> uboData;
					if (ubo.count("data") > 0)
					{
						for (auto& data : ubo["data"])
						{
							void* ptr = nullptr;
							auto& type = data["type"];
							if (type == "pointer")
							{
								if (data["var"].get<std::string>().compare("VP Matrix") == 0)
								{
									ptr = (void*)&layer.getCamera()->getCamera()->getViewProjection();
								}

							}
							if (type == "Float3")
							{
								layer.getJsonData().push_back(new glm::vec3(data["x"].get<float>(), data["y"].get<float>(), data["z"].get<float>()));
								ptr = (void*)&(*(glm::vec3*)layer.getJsonData().back())[0];
							}

							//TODO add more data types?

							uboData.push_back(ptr);
						}
						layer.getSceneData()[ResourceManagerInstance->getUBO().getAsset(uboName)] = uboData;
					}


					if (jsonFile.count("RendererCommands") > 0)
					{
						std::string stages[4] = { "init", "predraw", "postdraw", "exit" };

						for (int i = 0; i < 4; i++)
						{
							if (jsonFile["RendererCommands"].count(stages[i]) > 0)
							{
								for (auto& stage : jsonFile["RendererCommands"][stages[i]])
								{
									std::string type = stage["type"].get<std::string>();
									std::shared_ptr<RenderCommand> command;
									bool keepAlive = true; // Adding to shared ptrs to leave deletion to the smart ptr

									if (type.compare("ClearDepthColorBuffer") == 0)
									{
										command.reset(RenderCommand::clearDepthColorBuffer(keepAlive));
									}

									if (type.compare("ClearColor") == 0)
									{
										float r = stage["r"].get<float>();
										float g = stage["g"].get<float>();
										float b = stage["b"].get<float>();
										float a = stage["a"].get<float>();
										command.reset(RenderCommand::setClearColor(keepAlive, r, g, b, a));
									}

									if (type.compare("DepthTestLess") == 0)
									{
										bool enabled = stage["enabled"].get<bool>();
										command.reset(RenderCommand::setDepthTestLest(keepAlive, enabled));
									}

									if (type.compare("BackfaceCulling") == 0)
									{
										bool enabled = stage["enabled"].get<bool>();
										command.reset(RenderCommand::setBackfaceCulling(keepAlive, enabled));
									}

									if (type.compare("MinusOneAlphaBlending") == 0)
									{
										bool enabled = stage["enabled"].get<bool>();
										command.reset(RenderCommand::setMinusOneAlphaBlending(keepAlive, enabled));
									}

									if (type.compare("Debug") == 0)
									{
										bool enabled = stage["enabled"].get<bool>();
										command.reset(RenderCommand::setDebugOutput(keepAlive, enabled));
									}

									switch (i)
									{
									case 0:
										layer.getInitCommands().push_back(command);
										break;
									case 1:
										layer.getPredrawCommands().push_back(command);
										break;
									case 2:
										layer.getPostdrawCommands().push_back(command);
										break;
									case 3:
										layer.getExitCommands().push_back(command);
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	};
}