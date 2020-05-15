#include "engine_pch.h"
#include "data/json/JsonLoader.h"
#include "core/application.h"

namespace Engine
{
	HDC JsonLoader::hdc;
	HGLRC JsonLoader::glrc;

	std::shared_ptr<JsonModel> JsonLoader::loadJsonModelAsync(std::string filepath)
	{
		std::shared_ptr<JsonModel> model;
		model.reset(JsonModelLoader::loadModel(filepath));
		return model;
	}

	std::shared_ptr<AssimpModel> JsonLoader::loadAssimpModelAsync(std::string filepath)
	{
		std::shared_ptr<AssimpModel> model;
		model.reset(AssimpModelLoader::loadModel(filepath));
		return model;
	}

	std::shared_ptr<Shader> JsonLoader::loadShaderAsync(std::string path)
	{
		std::shared_ptr<Shader> sh;
		sh.reset(Shader::create(path));
		return sh;
	}

	std::shared_ptr<Texture> JsonLoader::loadTextureAsync(std::string path)
	{
		std::shared_ptr<Texture> tex;
		tex.reset(Texture::createFromFile(path));
		return tex;
	}

	//! Loads and populates layers defined in json files \param filepath path to the json file \param layer layer to be loaded
	void JsonLoader::loadJson(const std::string& filepath, JsonLayer& layer)
	{
#ifdef NG_DEBUG
		//Engine::JsonLog::loadLog(filepath);
#endif
		std::fstream file(filepath, std::ios::in);

		if (!file.is_open())
		{
			LogError("Could not open {0}", filepath);
			return;
		}

		nlohmann::json jsonFile;
		file >> jsonFile;

		Application::getInstance().getWindow()->getGraphicsContext()->unbindCurrentThread();
		if (jsonFile.count("Asyncload") > 0)
		{
			std::vector<std::string> names;
			int i = 0;
			if (jsonFile["Asyncload"].count("shaders") > 0)
			{
				// create vector of futures
				std::vector<std::future<std::shared_ptr<Shader>>> futures;
				
				
				for (auto& fnFilepath : jsonFile["Asyncload"]["shaders"])
				{
					// launch async here
					std::string name = fnFilepath["name"].get<std::string>();
					names.push_back(name);
					std::string path = fnFilepath["filepath"].get<std::string>();
					futures.push_back(std::async(std::launch::async, loadShaderAsync, path));
				}

				// Create extra loop to go over the vector of futures and add them to resource manager instance
				i = 0;
				for (auto& f : futures)
				{
					ResourceManagerInstance->addShaderAsync(names.at(i), f.get());
					i++;
				}
				futures.clear();
				names.clear();
			}
			if (jsonFile["Asyncload"].count("textures") > 0)
			{
				std::vector<std::future<std::shared_ptr<Texture>>> texFutures;
				for (auto& fnFilepath : jsonFile["Asyncload"]["textures"])
				{
					std::string name = fnFilepath["name"].get<std::string>();
					names.push_back(name);
					std::string path = fnFilepath["filepath"].get<std::string>();
					texFutures.push_back(std::async(std::launch::async, loadTextureAsync, path));
				}

				i = 0;
				for (auto& f : texFutures)
				{
					ResourceManagerInstance->addTextureAsync(names.at(i), f.get());
					i++;
				}

				// compile textures
				Application::getInstance().getWindow()->getGraphicsContext()->swapToCurrentThread();
				for (auto name : names) 
				{
					ResourceManagerInstance->getTexture().getAsset(name)->compile();
				}
				Application::getInstance().getWindow()->getGraphicsContext()->unbindCurrentThread();
				names.clear();
			}
			if (jsonFile["Asyncload"].count("models") > 0)
			{
				std::vector<std::future<std::shared_ptr<JsonModel>>> jsonModelFutures;
				std::vector<std::future<std::shared_ptr<AssimpModel>>> assimpModelFutures;
				std::vector<std::string> jsonNames;
				std::vector<std::string> assimpNames;
				for (auto& fnFilepath : jsonFile["Asyncload"]["models"])
				{
					std::string type = fnFilepath["type"].get<std::string>();
					if (type == "json")
					{
						//json model
						std::string name = fnFilepath["name"].get<std::string>();
						jsonNames.push_back(name);
						std::string path = fnFilepath["filepath"].get<std::string>();
						jsonModelFutures.push_back(std::async(std::launch::async, loadJsonModelAsync, path));
					}
					if (type == "assimp")
					{
						//assimp model
						std::string name = fnFilepath["name"].get<std::string>();
						assimpNames.push_back(name);
						std::string path = fnFilepath["filepath"].get<std::string>();
						assimpModelFutures.push_back(std::async(std::launch::async, loadAssimpModelAsync, path));
					}
				}

				i = 0;
				for (auto& f : jsonModelFutures)
				{
					ResourceManagerInstance->addJsonModelAsync(jsonNames.at(i), f.get());
					i++;
				}
				jsonModelFutures.clear();
				jsonNames.clear();

				i = 0;
				for (auto& f : assimpModelFutures)
				{
					ResourceManagerInstance->addAssimpModelAsync(assimpNames.at(i), f.get());
					i++;
				}
				assimpModelFutures.clear();
				assimpNames.clear();
			}
			Application::getInstance().getWindow()->getGraphicsContext()->swapToCurrentThread();
			if (jsonFile["Asyncload"].count("fonts") > 0)
			{
				std::unordered_map<std::string, unsigned int> fonts;
				for (auto& fnFilepath : jsonFile["Asyncload"]["fonts"])
				{
					std::string filepath = fnFilepath["filepath"].get<std::string>();
					int size = fnFilepath["charSize"].get<int>();
					fonts[filepath] = size;
					ResourceManagerInstance->getFontPaths().push_back(filepath);
				}
				if (!fonts.empty()) ResourceManagerInstance->populateCharacters(fonts);
			}
			if (jsonFile["Asyncload"].count("sounds") > 0)
			{
				for (auto& fnFilepath : jsonFile["Asyncload"]["sounds"])
				{
					std::string soundName = fnFilepath["name"].get<std::string>();
					bool b3d = fnFilepath["b3d"].get<bool>();
					bool loop = fnFilepath["loop"].get<bool>();
					bool stream = fnFilepath["stream"].get<bool>();
					float minD = fnFilepath["minDist"].get<float>();
					float maxDist = fnFilepath["maxDist"].get<float>();
					std::string roll = fnFilepath["rollOff"].get<std::string>();
					RollOff r;

					if (roll == "InverseTapered")
					{
						r = RollOff::InverseTapered;
					}
					else if (roll == "Linear")
					{
						r = RollOff::Linear;
					}
					else if (roll == "LinearSquared")
					{
						r = RollOff::LinearSquared;
					}
					layer.getSounds()[soundName] = ResourceManagerInstance->addSound(soundName, b3d, loop, stream, minD, maxDist, r);
				}

			}

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
						if (SDT.compare("Float") == 0) { uboLayout.addElement(ShaderDataType::Float); }
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
							if (data["var"].get<std::string>().compare("camPosition") == 0)
							{
								ptr = (void*)&layer.getCamera()->getCamera()->getPosition();
							}
							if (data["var"].get<std::string>().compare("cutOff") == 0)
							{
								ptr = (void*)&layer.getCamera()->getCamera()->getCutOff();
							}
							if (data["var"].get<std::string>().compare("camForward") == 0)
							{
								auto cam = layer.getCamera()->getCamera();
								auto cam3D = std::static_pointer_cast<Camera3D>(cam);
								ptr = (void*)&cam3D->getForward();
							}
						}
						if (type == "Float3")
						{
							layer.getJsonData().push_back(new glm::vec3(data["x"].get<float>(), data["y"].get<float>(), data["z"].get<float>()));
							ptr = (void*)&(*(glm::vec3*)layer.getJsonData().back())[0];
						}
						if (type == "Float")
						{
							layer.getJsonData().push_back(new float(data["x"].get<float>()));
							ptr = (void*)&(*(float*)layer.getJsonData().back());
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

	void JsonLoader::loadGameObjects(const std::string& filepath, JsonLayer& layer)
	{

#ifdef NG_DEBUG
		//Engine::JsonLog::loadLog(filepath);
#endif
		std::fstream file(filepath, std::ios::in);

		if (!file.is_open())
		{
			LogError("Could not open {0}", filepath);
			return;
		}

		nlohmann::json jsonFile;
		file >> jsonFile;

		if (jsonFile.count("GameObject") > 0)
		{
			for (auto& go : jsonFile["GameObject"])
			{
				std::string goName = "Name";
				if (go.count("name") > 0)
				{
					goName = go["name"].get<std::string>();
				}
				layer.getGameObjects()[goName] = std::make_shared<GameObject>(GameObject(goName));
				auto gameObject = layer.getGameObjects()[goName];

				if (go.count("material") > 0)
				{
					if (go["material"].count("model") > 0)
					{
						std::string meshType = go["material"]["type"].get<std::string>();

						if (meshType == "json")
						{
							std::shared_ptr<MaterialComponent> mat;
							std::string modelName = go["material"]["model"].get<std::string>();
							std::shared_ptr<JsonModel> model = ResourceManagerInstance->getJsonModels().getAsset(modelName);

							std::string shader = go["material"]["shader"].get<std::string>();

							ResourceManagerInstance->addVAO(goName + "VAO");
							ResourceManagerInstance->addVBO(goName + "VBO", model->vertices, sizeof(float) * model->verticesSize, model->shader->getBufferLayout());
							ResourceManagerInstance->addEBO(goName + "EBO", model->indices, sizeof(unsigned int) * model->indicesSize);

							ResourceManagerInstance->getVAO().getAsset(goName + "VAO")->
								setVertexBuffer(ResourceManagerInstance->getVBO().getAsset(goName + "VBO"));
							ResourceManagerInstance->getVAO().getAsset(goName + "VAO")->
								setIndexBuffer(ResourceManagerInstance->getEBO().getAsset(goName + "EBO"));

							ResourceManagerInstance->addMaterial(goName + "Mat",
								ResourceManagerInstance->getShader().getAsset(shader),
								ResourceManagerInstance->getVAO().getAsset(goName + "VAO"));

							mat = std::make_shared<MaterialComponent>
								(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(goName + "Mat")));
							mat->setTypeName(meshType);
							mat->setModelName(modelName);
							mat->setShaderName(shader);
							gameObject->addComponent(mat);
						}
						else if (meshType == "assimp")
						{
							std::string modelName = go["material"]["model"].get<std::string>();

							//process assimp model
							std::shared_ptr<AssimpModel> assimpModel = ResourceManagerInstance->getAssimpModels().getAsset(modelName);

							for (int i = 0; i < assimpModel->m_meshes.size(); i++)
							{
								std::shared_ptr<MaterialComponent> mat;

								Mesh* mesh = &assimpModel->m_meshes.at(i);
								mesh->m_shader = ResourceManagerInstance->getShader().getAsset(go["material"]["shader"].get<std::string>());

								//set each mesh up
									// add VAO
								ResourceManagerInstance->addVAO(goName + "VAO" + std::to_string(i));
								// add VBO
								ResourceManagerInstance->addVBO(goName + "VBO" + std::to_string(i),
									(float*)&mesh->m_vertices.at(0),
									mesh->m_vertices.size() * sizeof(VertexData),
									mesh->m_shader->getBufferLayout());
								// add EBO
								ResourceManagerInstance->addEBO(goName + "EBO" + std::to_string(i),
									&mesh->m_indices.at(0),
									mesh->m_indices.size() * sizeof(unsigned int));
								// link VBO to VAO
								ResourceManagerInstance->getVAO().getAsset(goName + "VAO" + std::to_string(i))->
									setVertexBuffer(ResourceManagerInstance->getVBO().getAsset(goName + "VBO" + std::to_string(i)));
								// link EBO to VAO
								ResourceManagerInstance->getVAO().getAsset(goName + "VAO" + std::to_string(i))->
									setIndexBuffer(ResourceManagerInstance->getEBO().getAsset(goName + "EBO" + std::to_string(i)));

								mesh->setupMesh(mesh->m_vertices.at(0), mesh->m_indices.at(0));

								// add material
								ResourceManagerInstance->addMaterial(goName + "Mat" + std::to_string(i),
									mesh->m_shader,
									ResourceManagerInstance->getVAO().getAsset(goName + "VAO" + std::to_string(i)));

								mat = std::make_shared<MaterialComponent>
									(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(goName + "Mat" + std::to_string(i))));
								mat->setTypeName(meshType);
								mat->setModelName(modelName);
								mat->setShaderName(go["material"]["shader"].get<std::string>());
								gameObject->addComponent(mat);
							}
						}

						if (go.count("texture") > 0)
						{
							std::shared_ptr<TextureComponent> tex;
							std::string texName = go["texture"]["diffuse"].get<std::string>();
							std::string specularTexName = go["texture"]["specular"].get<std::string>();
							std::string normalTexName = go["texture"]["normal"].get<std::string>();
							std::string parallaxTexName = go["texture"]["parallax"].get<std::string>();

							tex = std::make_shared<TextureComponent>
								(TextureComponent(ResourceManagerInstance->getTexture().getAsset(texName)->getSlot()));

							tex->setDiffuseTextureName(texName);
							tex->setNormalTextureName(normalTexName);
							tex->setParallaxTextureName(parallaxTexName);
							tex->setSpecularTextureName(specularTexName);

							gameObject->addComponent(tex);

							if (specularTexName.compare("none") != 0)
							{
								tex->assignSpecularTexture(ResourceManagerInstance->getTexture().getAsset(specularTexName)->getSlot());
							}
							if (normalTexName.compare("none") != 0)
							{
								tex->assignNormalTexture(ResourceManagerInstance->getTexture().getAsset(normalTexName)->getSlot());
							}
							if (parallaxTexName.compare("none") != 0)
							{
								tex->assignParallaxTexture(ResourceManagerInstance->getTexture().getAsset(parallaxTexName)->getSlot());
							}
							
						}
						
					}

					else if (go["material"].count("text") > 0)
					{
						std::shared_ptr<MaterialComponent> textMat;
						std::shared_ptr<TextureComponent> textTex;
						std::shared_ptr<ColourComponent> textCol;
						std::shared_ptr<TextComponent> textLabel;

						std::string text = go["material"]["text"].get<std::string>();
						std::string font = go["material"]["font"].get<std::string>();
						int charSize = go["material"]["charSize"].get<int>();
						float r = go["material"]["colour"]["r"].get<float>();
						float g = go["material"]["colour"]["g"].get<float>();
						float b = go["material"]["colour"]["b"].get<float>();
						
						textLabel = std::make_shared<TextComponent>(TextComponent(goName, font, charSize, text));
						
						gameObject->addComponent(textLabel);

						auto& mat = textLabel->getLabel()->getMaterial();

						textMat = std::make_shared<MaterialComponent>(MaterialComponent(mat));
						gameObject->addComponent(textMat);

						textTex = std::make_shared<TextureComponent>
							(TextureComponent(ResourceManagerInstance->getFontTexture()->getSlot()));
						gameObject->addComponent(textTex);

						textCol = std::make_shared<ColourComponent>(ColourComponent(r, g, b));
						gameObject->addComponent(textCol);
					}
				}

				if (go.count("position") > 0)
				{
					std::shared_ptr<PositionComponent> pos;
					glm::vec3 translation(go["position"]["translation"]["x"].get<float>(), go["position"]["translation"]["y"].get<float>(), go["position"]["translation"]["z"].get<float>());
					glm::vec3 rotation(go["position"]["rotation"]["x"].get<float>(), go["position"]["rotation"]["y"].get<float>(), go["position"]["rotation"]["z"].get<float>());
					glm::vec3 scale(go["position"]["scale"]["x"].get<float>(), go["position"]["scale"]["y"].get<float>(), go["position"]["scale"]["z"].get<float>());

					pos = std::make_shared<PositionComponent>(PositionComponent(translation, rotation, scale));
					gameObject->addComponent(pos);
				}

				if (go.count("velocity") > 0)
				{
					std::shared_ptr<VelocityComponent> vel;
					glm::vec3 linear(go["velocity"]["linear"]["x"].get<float>(), go["velocity"]["linear"]["y"].get<float>(), go["velocity"]["linear"]["z"].get<float>());
					glm::vec3 angular(go["velocity"]["angular"]["x"].get<float>(), go["velocity"]["angular"]["y"].get<float>(), go["velocity"]["angular"]["z"].get<float>());				

					if (go.count("oscillate") > 0)
					{
						std::shared_ptr<OscillateComponent> osc;
						OscillateComponent::State state = OscillateComponent::State::DOWN;

						auto stateStr = go["oscillate"]["state"].get<std::string>();
						if (stateStr.compare("DOWN") == 0) state = OscillateComponent::State::DOWN;
						if (stateStr.compare("STOPPED") == 0) state = OscillateComponent::State::STOPPED;
						if (stateStr.compare("UP") == 0) state = OscillateComponent::State::UP;

						float currTime = go["oscillate"]["current time"].get<float>();
						float maxTime = go["oscillate"]["max time"].get<float>();
						bool setT = go["oscillate"]["set texture"].get<bool>();

						osc = std::make_shared<OscillateComponent>
							(OscillateComponent(state, currTime, maxTime, setT, vel->getLinear()));
						gameObject->addComponent(osc);
					}

					if (go.count("controller") > 0)
					{
						std::shared_ptr<ControllerComponent> ctr;
						float moveSpeed = go["controller"]["moveSpeed"].get<float>();
						float rotationSpeed = go["controller"]["rotationSpeed"].get<float>();

						ctr = std::make_shared<ControllerComponent>
							(ControllerComponent(moveSpeed, rotationSpeed));
						gameObject->addComponent(ctr);
					}

					if (go.count("AI") > 0)
					{
						std::shared_ptr<AIComponent> ai;

						float stop = go["AI"]["stopDist"].get<float>();
						std::string type = go["AI"]["aiType"].get<std::string>();
						std::string script = go["AI"]["script"].get<std::string>();
						std::string pathType = go["AI"]["pathType"].get<std::string>();
						std::vector<glm::vec3> pathWaypoints;

						for (auto& waypoint : go["AI"]["waypoints"])
						{
							float x = waypoint["x"].get<float>();
							float y = waypoint["y"].get<float>();
							float z = waypoint["z"].get<float>();

							pathWaypoints.push_back(glm::vec3(x, y, z));
						}

						ai = std::make_shared<AIComponent>(AIComponent(stop, type, script, pathType));
						ai->registerClass();
						ai->doFile("../scripts/" + script + ".lua", type, "update");

						for (int j = 0; j < pathWaypoints.size(); j++)
						{
							ai->addPath(pathWaypoints[j].x, pathWaypoints[j].y, pathWaypoints[j].z);
						}

						gameObject->addComponent(ai);
					}

					vel = std::make_shared<VelocityComponent>
						(VelocityComponent(linear, angular));
					gameObject->addComponent(vel);
				}
			}
		}
	}

	void JsonLoader::loadImGui(const std::string & filepath, ImGuiLayer & layer)
	{
		std::fstream file(filepath, std::ios::in);

		if (!file.is_open())
		{
			LogError("Could not open {0}", filepath);
			return;
		}

		nlohmann::json jsonFile;
		file >> jsonFile;

		if (jsonFile.count("Functions") > 0)
		{
			if (jsonFile["Functions"]["Material"].get<bool>() == true)
			{
				layer.addImGuiFunction([&](JsonLayer* lay) 
				{
					if (!layer.is2D())
					{
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
									for (int i = 0; i < layer.getJsonModelsNames().size(); i++)
									{
										bool selected = (meshItem == layer.getJsonModelsNames()[i]);

										if (ImGui::Selectable(layer.getJsonModelsNames()[i].c_str(), selected))
										{
											meshItem = layer.getJsonModelsNames()[i].c_str();
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
									for (int i = 0; i < layer.getAssimpModelsNames().size(); i++)
									{
										bool selected = (meshItem == layer.getAssimpModelsNames()[i]);

										if (ImGui::Selectable(layer.getAssimpModelsNames()[i].c_str(), selected))
										{
											meshItem = layer.getAssimpModelsNames()[i].c_str();
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
								for (int i = 0; i < layer.getShaderNames().size(); i++)
								{
									bool selected = (shaderName == layer.getShaderNames()[i]);

									if (ImGui::Selectable(layer.getShaderNames()[i].c_str(), selected))
									{
										shaderName = layer.getShaderNames()[i].c_str();
									}

									if (selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}

							if (ImGui::Button("Add"))
							{
								// check if component does not exist yet
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<MaterialComponent>() == nullptr)
								{
									// set material up
									if (currentItem == "Json")
									{
										std::shared_ptr<MaterialComponent> mat;
										std::shared_ptr<JsonModel> model = ResourceManagerInstance->getJsonModels().getAsset(meshItem);

										ResourceManagerInstance->addVAO(layer.getGOName() + "VAO");
										ResourceManagerInstance->addVBO(layer.getGOName() + "VBO", model->vertices, sizeof(float) * model->verticesSize, model->shader->getBufferLayout());
										ResourceManagerInstance->addEBO(layer.getGOName() + "EBO", model->indices, sizeof(unsigned int) * model->indicesSize);

										ResourceManagerInstance->getVAO().getAsset(layer.getGOName() + "VAO")->
											setVertexBuffer(ResourceManagerInstance->getVBO().getAsset(layer.getGOName() + "VBO"));
										ResourceManagerInstance->getVAO().getAsset(layer.getGOName() + "VAO")->
											setIndexBuffer(ResourceManagerInstance->getEBO().getAsset(layer.getGOName() + "EBO"));

										ResourceManagerInstance->addMaterial(layer.getGOName() + "Mat",
											ResourceManagerInstance->getShader().getAsset(shaderName),
											ResourceManagerInstance->getVAO().getAsset(layer.getGOName() + "VAO"));

										mat = std::make_shared<MaterialComponent>
											(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(layer.getGOName() + "Mat")));
										mat->setTypeName("json");
										mat->setModelName(meshItem);
										mat->setShaderName(shaderName);
										lay->getGameObjects()[layer.getGOName()]->addComponent(mat);
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
											ResourceManagerInstance->addVAO(layer.getGOName() + "VAO" + std::to_string(i));
											// add VBO
											ResourceManagerInstance->addVBO(layer.getGOName() + "VBO" + std::to_string(i),
												(float*)&mesh->m_vertices.at(0),
												mesh->m_vertices.size() * sizeof(VertexData),
												mesh->m_shader->getBufferLayout());
											// add EBO
											ResourceManagerInstance->addEBO(layer.getGOName() + "EBO" + std::to_string(i),
												&mesh->m_indices.at(0),
												mesh->m_indices.size() * sizeof(unsigned int));
											// link VBO to VAO
											ResourceManagerInstance->getVAO().getAsset(layer.getGOName() + "VAO" + std::to_string(i))->
												setVertexBuffer(ResourceManagerInstance->getVBO().getAsset(layer.getGOName() + "VBO" + std::to_string(i)));
											// link EBO to VAO
											ResourceManagerInstance->getVAO().getAsset(layer.getGOName() + "VAO" + std::to_string(i))->
												setIndexBuffer(ResourceManagerInstance->getEBO().getAsset(layer.getGOName() + "EBO" + std::to_string(i)));

											mesh->setupMesh(mesh->m_vertices.at(0), mesh->m_indices.at(0));

											// add material
											ResourceManagerInstance->addMaterial(layer.getGOName() + "Mat" + std::to_string(i),
												mesh->m_shader,
												ResourceManagerInstance->getVAO().getAsset(layer.getGOName() + "VAO" + std::to_string(i)));

											mat = std::make_shared<MaterialComponent>
												(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(layer.getGOName() + "Mat" + std::to_string(i))));
											mat->setTypeName("assimp");
											mat->setModelName(meshItem);
											mat->setShaderName(shaderName);
											lay->getGameObjects()[layer.getGOName()]->addComponent(mat);
										}
									}
								}
							}
							ImGui::SameLine(100);
							if (ImGui::Button("Remove"))
							{
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<MaterialComponent>() != nullptr)
								{
									// set material up
									if (currentItem == "Json")
									{
										ResourceManagerInstance->getVAO().remove(layer.getGOName() + "VAO");
										ResourceManagerInstance->getVBO().remove(layer.getGOName() + "VBO");
										ResourceManagerInstance->getEBO().remove(layer.getGOName() + "EBO");
										ResourceManagerInstance->getMaterial().remove(layer.getGOName() + "Mat");

										lay->getGameObjects()[layer.getGOName()]->removeComponent(lay->getGameObjects()[layer.getGOName()]->getComponent<MaterialComponent>());
									}
									else if (currentItem == "Assimp")
									{
										std::shared_ptr<AssimpModel> assimpModel = ResourceManagerInstance->getAssimpModels().getAsset(meshItem);

										for (int i = 0; i < assimpModel->m_meshes.size(); i++)
										{
											ResourceManagerInstance->getVAO().remove(layer.getGOName() + "VAO" + std::to_string(i));
											ResourceManagerInstance->getVBO().remove(layer.getGOName() + "VBO");
											ResourceManagerInstance->getEBO().remove(layer.getGOName() + "EBO" + std::to_string(i));
											ResourceManagerInstance->getMaterial().remove(layer.getGOName() + "Mat" + std::to_string(i));

											lay->getGameObjects()[layer.getGOName()]->removeComponent(lay->getGameObjects()[layer.getGOName()]->getComponent<MaterialComponent>());
										}
									}
								}
								else
								{
									LogWarn("Component did not exist anyway!");
								}
							}
						}
					}
					
				});
			}
			if (jsonFile["Functions"]["Texture"].get<bool>() == true)
			{
				layer.addImGuiFunction([&](JsonLayer* lay)
				{
					if (!layer.is2D())
					{
						if (ImGui::CollapsingHeader("Texture"))
						{
							static const char* tex = "";
							static const char* normTex = "none";
							static const char* parallTex = "none";
							static const char* specTex = "none";

							if (ImGui::BeginCombo("Diffuse texture", tex))
							{
								for (int i = 0; i < layer.getTexturesNames().size(); i++)
								{
									bool selected = (tex == layer.getTexturesNames()[i]);

									if (ImGui::Selectable(layer.getTexturesNames()[i].c_str(), selected))
									{
										tex = layer.getTexturesNames()[i].c_str();
									}

									if (selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}

							if (ImGui::BeginCombo("Normal texture", normTex))
							{
								for (int i = 0; i < layer.getTexturesNames().size(); i++)
								{
									bool selected = (normTex == layer.getTexturesNames()[i]);

									if (ImGui::Selectable(layer.getTexturesNames()[i].c_str(), selected))
									{
										normTex = layer.getTexturesNames()[i].c_str();
									}

									if (selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}

							if (ImGui::BeginCombo("Parallax texture", parallTex))
							{
								for (int i = 0; i < layer.getTexturesNames().size(); i++)
								{
									bool selected = (parallTex == layer.getTexturesNames()[i]);

									if (ImGui::Selectable(layer.getTexturesNames()[i].c_str(), selected))
									{
										parallTex = layer.getTexturesNames()[i].c_str();
									}

									if (selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}

							if (ImGui::BeginCombo("Specular texture", specTex))
							{
								for (int i = 0; i < layer.getTexturesNames().size(); i++)
								{
									bool selected = (specTex == layer.getTexturesNames()[i]);

									if (ImGui::Selectable(layer.getTexturesNames()[i].c_str(), selected))
									{
										specTex = layer.getTexturesNames()[i].c_str();
									}

									if (selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}

							if (ImGui::Button("Add"))
							{
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<TextureComponent>() == nullptr)
								{
									// TODO more textures
									std::shared_ptr<TextureComponent> texComp;
									texComp = std::make_shared<TextureComponent>
										(TextureComponent(ResourceManagerInstance->getTexture().getAsset(tex)->getSlot()));

									lay->getGameObjects()[layer.getGOName()]->addComponent(texComp);

									if (normTex != "none")
										texComp->assignNormalTexture(ResourceManagerInstance->getTexture().getAsset(normTex)->getSlot());

									if (parallTex != "none")
										texComp->assignParallaxTexture(ResourceManagerInstance->getTexture().getAsset(parallTex)->getSlot());

									if (specTex != "none")
										texComp->assignSpecularTexture(ResourceManagerInstance->getTexture().getAsset(specTex)->getSlot());

									texComp->setDiffuseTextureName(tex);
									texComp->setNormalTextureName(normTex);
									texComp->setParallaxTextureName(parallTex);
									texComp->setSpecularTextureName(specTex);
								}
								else
								{
									LogWarn("Component already exists!");
								}
							}
							ImGui::SameLine(100);
							if (ImGui::Button("Change"))
							{
								auto comp = lay->getGameObjects()[layer.getGOName()]->getComponent<TextureComponent>();
								if (comp != nullptr)
								{
									comp->setTexture(ResourceManagerInstance->getTexture().getAsset(tex)->getSlot());

									if (normTex != "none")
										comp->assignNormalTexture(ResourceManagerInstance->getTexture().getAsset(normTex)->getSlot());

									if (parallTex != "none")
										comp->assignParallaxTexture(ResourceManagerInstance->getTexture().getAsset(parallTex)->getSlot());

									if (specTex != "none")
										comp->assignSpecularTexture(ResourceManagerInstance->getTexture().getAsset(specTex)->getSlot());

									comp->setDiffuseTextureName(tex);
									comp->setNormalTextureName(normTex);
									comp->setParallaxTextureName(parallTex);
									comp->setSpecularTextureName(specTex);

								}
								else
								{
									LogError("Can't change - add first!");
								}
							}
							ImGui::SameLine(200);
							if (ImGui::Button("Remove"))
							{
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<TextureComponent>() != nullptr)
								{
									auto comp = lay->getGameObjects()[layer.getGOName()]->getComponent<TextureComponent>();
									lay->getGameObjects()[layer.getGOName()]->removeComponent(comp);
								}
								else
								{
									LogWarn("Component did not exist anyway!");
								}
							}
							ImGui::SameLine(300);
							if (ImGui::Button("Reset"))
							{
								auto comp = lay->getGameObjects()[layer.getGOName()]->getComponent<TextureComponent>();
								if (comp != nullptr)
								{
									tex = "";
									normTex = "none";
									parallTex = "none";
									specTex = "none";
								}
								else
								{
									LogError("Can't reset - add first!");
								}
							}
						}
					}				
				});
			}
			if (jsonFile["Functions"]["Position"].get<bool>() == true)
			{
				layer.addImGuiFunction([&](JsonLayer* lay)
				{
					if (ImGui::CollapsingHeader("Position"))
					{
						if (lay->getGameObjects().find(layer.getGOName()) != lay->getGameObjects().end())
						{
							glm::vec3& pos = lay->getGameObjects()[layer.getGOName()]->getComponent<PositionComponent>()->getCurrentPosition();
							glm::vec3& rot = lay->getGameObjects()[layer.getGOName()]->getComponent<PositionComponent>()->getCurrentRotation();
							glm::vec3& scl = lay->getGameObjects()[layer.getGOName()]->getComponent<PositionComponent>()->getCurrentScale();

							ImGui::InputFloat3("Position", &pos.x, 2);
							ImGui::InputFloat3("Rotation", &rot.x, 2);
							ImGui::InputFloat3("Scale", &scl.x, 2);

							lay->getGameObjects()[layer.getGOName()]->getComponent<PositionComponent>()->setPosition(pos);
							lay->getGameObjects()[layer.getGOName()]->getComponent<PositionComponent>()->setRotation(rot);
							lay->getGameObjects()[layer.getGOName()]->getComponent<PositionComponent>()->setScale(scl);
						}
					}
				});
			}
			if (jsonFile["Functions"]["Velocity"].get<bool>() == true)
			{
				layer.addImGuiFunction([&](JsonLayer* lay)
				{
					if (!layer.is2D())
					{
						if (ImGui::CollapsingHeader("Velocity"))
						{
							static glm::vec3 linear = glm::vec3(0.0f);
							static glm::vec3 angular = glm::vec3(0.0f);

							ImGui::InputFloat3("Linear", &linear.x, 3);
							ImGui::InputFloat3("Angular", &angular.x, 3);

							if (ImGui::Button("Add"))
							{
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<VelocityComponent>() == nullptr)
								{
									std::shared_ptr<VelocityComponent> vel;
									vel = std::make_shared<VelocityComponent>(VelocityComponent(linear, angular));
									lay->getGameObjects()[layer.getGOName()]->addComponent(vel);
								}
							}
							ImGui::SameLine(100);
							if (ImGui::Button("Change"))
							{
								auto comp = lay->getGameObjects()[layer.getGOName()]->getComponent<VelocityComponent>();
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
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<VelocityComponent>())
								{
									auto comp = lay->getGameObjects()[layer.getGOName()]->getComponent<VelocityComponent>();
									lay->getGameObjects()[layer.getGOName()]->removeComponent(comp);
								}
								else
								{
									LogWarn("Component did not exist anyway!");
								}
							}
						}
					}
				});
			}
			if (jsonFile["Functions"]["Controller"].get<bool>() == true)
			{
				layer.addImGuiFunction([&](JsonLayer* lay)
				{
					if (!layer.is2D())
					{
						if (ImGui::CollapsingHeader("Controller"))
						{
							static float moveSpeed = 0.0f;
							static float rotationSpeed = 0.0f;

							ImGui::InputFloat("Move speed", &moveSpeed, 0.01f, 1.0f, 2);
							ImGui::InputFloat("Rotation speed", &rotationSpeed, 0.01f, 1.0f, 2);

							if (ImGui::Button("Add"))
							{
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<ControllerComponent>() == nullptr)
								{
									std::shared_ptr<ControllerComponent> ctr;
									ctr = std::make_shared<ControllerComponent>
										(ControllerComponent(moveSpeed, rotationSpeed));
									lay->getGameObjects()[layer.getGOName()]->addComponent(ctr);
								}
							}
							ImGui::SameLine(100);
							if (ImGui::Button("Change"))
							{
								auto comp = lay->getGameObjects()[layer.getGOName()]->getComponent<ControllerComponent>();
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
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<ControllerComponent>())
								{
									auto comp = lay->getGameObjects()[layer.getGOName()]->getComponent<ControllerComponent>();
									lay->getGameObjects()[layer.getGOName()]->removeComponent(comp);
								}
								else
								{
									LogWarn("Component did not exist anyway!");
								}
							}
						}
					}				
				});
			}
			if (jsonFile["Functions"]["Oscillate"].get<bool>() == true)
			{
				layer.addImGuiFunction([&](JsonLayer* lay)
				{
					if (!layer.is2D())
					{
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
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<OscillateComponent>() == nullptr)
								{
									std::shared_ptr<OscillateComponent> osc;
									std::shared_ptr<VelocityComponent> vel = lay->getGameObjects()[layer.getGOName()]->getComponent<VelocityComponent>();

									std::string stateStr = currentItem;

									if (stateStr.compare("Stopped") == 0) state = OscillateComponent::State::STOPPED;
									if (stateStr.compare("Down") == 0) state = OscillateComponent::State::DOWN;
									if (stateStr.compare("Up") == 0) state = OscillateComponent::State::UP;

									osc = std::make_shared<OscillateComponent>
										(OscillateComponent(state, currTime, maxTime, setTexture, vel->getLinear()));
									lay->getGameObjects()[layer.getGOName()]->addComponent(osc);
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
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<OscillateComponent>())
								{
									auto comp = lay->getGameObjects()[layer.getGOName()]->getComponent<OscillateComponent>();
									lay->getGameObjects()[layer.getGOName()]->removeComponent(comp);
								}
								else
								{
									LogWarn("Component did not exist anyway!");
								}
							}
						}
					}					
				});
			}
			if (jsonFile["Functions"]["AI"].get<bool>() == true)
			{
				layer.addImGuiFunction([&](JsonLayer* lay)
				{
					if (!layer.is2D())
					{
						if (ImGui::CollapsingHeader("AI"))
						{
							static float stopDist;
							static char aiType[32] = "";
							static char scriptName[32] = "";

							static float pathX = 0.0f;
							static float pathY = 0.0f;
							static float pathZ = 0.0f;

							ImGui::InputFloat("Stop distance", &stopDist, 0.01f, 0.1f, 2);
							ImGui::InputText("AI Type", aiType, IM_ARRAYSIZE(aiType));
							
							static const char* pathType = "";

							if (ImGui::BeginCombo("Path Type", pathType))
							{
								for (int i = 0; i < 3; i++)
								{
									bool selected = (pathType == layer.getPathTypes()[i]);

									if (ImGui::Selectable(layer.getPathTypes()[i].c_str(), selected))
									{
										pathType = layer.getPathTypes()[i].c_str();
									}

									if (selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}

							ImGui::InputText("", scriptName, IM_ARRAYSIZE(scriptName));
							ImGui::SameLine();
							ImGui::Text(".lua");


							if (lay->getGameObjects()[layer.getGOName()]->getComponent<AIComponent>())
							{
								static const char* selectedPath = "";
								static int selectedPathIndex;

								if (ImGui::BeginCombo("Path", selectedPath))
								{
									std::vector<const char*> paths;
									int pathSize = lay->getGameObjects()[layer.getGOName()]->getComponent<AIComponent>()->numPath();

									for (int i = 0; i < pathSize; i++)
									{			
										std::string pathNum = std::to_string(i);
										std::string currentPath = "Path " + pathNum;
										paths.push_back(currentPath.c_str());

										bool selected = (selectedPath == paths[i]);

										if (ImGui::Selectable(paths[i], selected))
										{
											selectedPath = paths[i];
											selectedPathIndex = i;

											pathX = lay->getGameObjects()[layer.getGOName()]->getComponent<AIComponent>()->pathPosX(selectedPathIndex);
											pathY = lay->getGameObjects()[layer.getGOName()]->getComponent<AIComponent>()->pathPosY(selectedPathIndex);
											pathZ = lay->getGameObjects()[layer.getGOName()]->getComponent<AIComponent>()->pathPosZ(selectedPathIndex);
										}

										if (selected)
											ImGui::SetItemDefaultFocus();
									}
									ImGui::EndCombo();
								}

								ImGui::InputFloat("X", &pathX, 1);
								ImGui::InputFloat("Y", &pathY, 1);
								ImGui::InputFloat("Z", &pathZ, 1);

								if (ImGui::Button("Add Path")) // Add to the Path
								{
									lay->getGameObjects()[layer.getGOName()]->getComponent<AIComponent>()->addPath(pathX, pathY, pathZ);
								}
								ImGui::SameLine(100);
								if (ImGui::Button("Change Path")) // Change the Path
								{
									lay->getGameObjects()[layer.getGOName()]->getComponent<AIComponent>()->changePath(selectedPathIndex, pathX, pathY, pathZ);
								}
							}
								
							if (ImGui::Button("Add"))
							{
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<AIComponent>() == nullptr)
								{
									std::shared_ptr<AIComponent> ai;
									std::string scrName = scriptName;

									LogInfo(pathType);

									ai = std::make_shared<AIComponent>(AIComponent(stopDist, aiType, scriptName, pathType)); ////////////////////////////////////////
									ai->registerClass();
									ai->doFile("../scripts/" + scrName + ".lua", aiType, "update");

									lay->getGameObjects()[layer.getGOName()]->addComponent(ai);
								}
								else
								{
									LogWarn("Component already exists!");
								}

							}
							ImGui::SameLine(100.0f);
							if (ImGui::Button("Remove"))
							{
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<AIComponent>())
								{
									auto comp = lay->getGameObjects()[layer.getGOName()]->getComponent<AIComponent>();
									lay->getGameObjects()[layer.getGOName()]->removeComponent(comp);
								}
								else
								{
									LogWarn("Component did not exist anyway!");
								}
							}
						}
					}
				});
			}

			if (jsonFile["Functions"]["Rigidbody"].get<bool>() == true)
			{
				layer.addImGuiFunction([&](JsonLayer* lay)
				{
					if (!layer.is2D())
					{
						/*if (ImGui::CollapsingHeader("Rigidbody"))
					{
						std::vector<const char*> types;
						types.push_back("Static");
						types.push_back("Kinematic");
						types.push_back("Dynamic");

						static const char* currentItem = types[types.size() - 1];
						static bool grav;

						if (ImGui::BeginCombo("Type", currentItem))
						{
							for (int i = 0; i < types.size(); i++)
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

						ImGui::Checkbox("Gravity", &grav);

						if (ImGui::Button("Add"))
						{
							if (lay->getGameObjects()[layer.getGOName()]->getComponent<RigidBodyComponent>() == nullptr)
							{
								std::shared_ptr<RigidBodyComponent> rb;
								rp3d::BodyType t;

								if (currentItem == "Static") t = rp3d::BodyType::STATIC;
								if (currentItem == "Kinematic") t = rp3d::BodyType::KINEMATIC;
								if (currentItem == "Dynamic") t = rp3d::BodyType::DYNAMIC;

								rb = std::make_shared<RigidBodyComponent>(RigidBodyComponent(t, grav));

								lay->getGameObjects()[layer.getGOName()]->addComponent(rb);
							}
							else
							{
								LogWarn("Component already exists!");
							}

						}
						ImGui::SameLine(100.0f);
						if (ImGui::Button("Remove"))
						{
							if (lay->getGameObjects()[layer.getGOName()]->getComponent<RigidBodyComponent>())
							{
								auto comp = lay->getGameObjects()[layer.getGOName()]->getComponent<RigidBodyComponent>();
								lay->getGameObjects()[layer.getGOName()]->removeComponent(comp);
							}
							else
							{
								LogWarn("Component did not exist anyway!");
							}

						}
					}*/
					}
					
				});
			}

			if (jsonFile["Functions"]["Collider"].get<bool>() == true)
			{
				layer.addImGuiFunction([&](JsonLayer* lay)
				{
					if (!layer.is2D())
					{
						/*if (ImGui::CollapsingHeader("Collider"))
						{
							std::vector<const char*> types;
							types.push_back("Box");
							types.push_back("Capsule");
							types.push_back("Sphere");
							types.push_back("Terrain");

							static const char* currentItem = types[0];

							if (ImGui::BeginCombo("Type", currentItem))
							{
								for (int i = 0; i < types.size(); i++)
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
						}*/
					}
									
				});
			}

			if (jsonFile["Functions"]["Text"].get<bool>() == true)
			{
				layer.addImGuiFunction([&](JsonLayer* lay)
				{
					if (layer.is2D())
					{
						if (ImGui::CollapsingHeader("Text"))
						{
							static char tText[32];
							static float r = 255.0f;
							static float g = 255.0f;
							static float b = 255.0f;

							static ImVec4 color = ImColor(r, g, b, 255.0f);

							static const char* currentItem = ResourceManagerInstance->getFontPaths()[0].c_str();

							if (ImGui::BeginCombo("Type", currentItem))
							{
								for (int i = 0; i < ResourceManagerInstance->getFontPaths().size(); i++)
								{
									bool selected = (currentItem == ResourceManagerInstance->getFontPaths()[i]);

									if (ImGui::Selectable(ResourceManagerInstance->getFontPaths()[i].c_str(), selected))
									{
										currentItem = ResourceManagerInstance->getFontPaths()[i].c_str();
									}

									if (selected)
										ImGui::SetItemDefaultFocus();
								}

								ImGui::EndCombo();
							}

							ImGui::InputText("Text label", tText, IM_ARRAYSIZE(tText));
							ImGui::ColorPicker3("Text color", (float*)&color);

							if (ImGui::Button("Add"))
							{
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<TextComponent>() == nullptr)
								{
									std::shared_ptr<TextComponent> tComp;
									tComp = std::make_shared<TextComponent>(TextComponent(layer.getGOName(), currentItem, 32, tText));

									auto mat = tComp->getLabel()->getMaterial();
									std::shared_ptr<MaterialComponent> mComp;
									mComp = std::make_shared<MaterialComponent>(mat);

									std::shared_ptr<TextureComponent> textTex;
									textTex = std::make_shared<TextureComponent>
										(TextureComponent(ResourceManagerInstance->getFontTexture()->getSlot()));

									std::shared_ptr<ColourComponent> cComp;
									cComp = std::make_shared<ColourComponent>(ColourComponent(color.x, color.y, color.z));

									lay->getGameObjects()[layer.getGOName()]->addComponent(tComp);
									lay->getGameObjects()[layer.getGOName()]->addComponent(mComp);
									lay->getGameObjects()[layer.getGOName()]->addComponent(cComp);
									lay->getGameObjects()[layer.getGOName()]->addComponent(textTex);
								}
								else
								{
									LogWarn("Component already exists!");
								}

							}
							ImGui::SameLine(100);
							if (ImGui::Button("Remove"))
							{
								if (lay->getGameObjects()[layer.getGOName()]->getComponent<TextComponent>())
								{
									ResourceManagerInstance->getVAO().remove(layer.getGOName() + "VAO");
									ResourceManagerInstance->getVBO().remove(layer.getGOName() + "VBO");
									ResourceManagerInstance->getEBO().remove(layer.getGOName() + "EBO");
									ResourceManagerInstance->getMaterial().remove(layer.getGOName() + "Mat");

									lay->getGameObjects()[layer.getGOName()]->removeComponent(lay->getGameObjects()[layer.getGOName()]->getComponent<TextComponent>());
									lay->getGameObjects()[layer.getGOName()]->removeComponent(lay->getGameObjects()[layer.getGOName()]->getComponent<TextureComponent>());
									lay->getGameObjects()[layer.getGOName()]->removeComponent(lay->getGameObjects()[layer.getGOName()]->getComponent<ColourComponent>());
									lay->getGameObjects()[layer.getGOName()]->removeComponent(lay->getGameObjects()[layer.getGOName()]->getComponent<MaterialComponent>());

								}
								else
								{
									LogWarn("Text component did not exist anyway!");
								}
							}
						}
					}
				});
			}
		}
	}
}