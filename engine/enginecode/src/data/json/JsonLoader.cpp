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
			if (jsonFile["Asyncload"].count("fonts") > 0)
			{
				for (auto& fnFilepath : jsonFile["Asyncload"]["shaders"])
				{
					//TODO add fonts
				}
			}
		}
		Application::getInstance().getWindow()->getGraphicsContext()->swapToCurrentThread();

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
							//bool tess = go["material"]["tess"].get<bool>();

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
									ResourceManagerInstance->getShader().getAsset(go["material"]["shader"].get<std::string>()),
									ResourceManagerInstance->getVAO().getAsset(goName + "VAO" + std::to_string(i)));

								 mat = std::make_shared<MaterialComponent>
									(MaterialComponent(ResourceManagerInstance->getMaterial().getAsset(goName + "Mat" + std::to_string(i))));
								 gameObject->addComponent(mat);
							}
						}

						if (go.count("texture") > 0)
						{
							std::shared_ptr<TextureComponent> tex;
							std::string texName = go["texture"]["name"].get<std::string>();

							tex = std::make_shared<TextureComponent>
								(TextureComponent(ResourceManagerInstance->getTexture().getAsset(texName)->getSlot()));
							gameObject->addComponent(tex);
						}
					}
					//TODO add text
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

					vel = std::make_shared<VelocityComponent>
						(VelocityComponent(linear, angular));
					gameObject->addComponent(vel);

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
							if (data["var"].get<std::string>().compare("Camera Position") == 0)
							{
								ptr = (void*)&layer.getCamera()->getCamera()->getPosition(); // Get the position of the camera.
							}
							if (data["var"].get<std::string>().compare("VP Matrix") == 0)
							{
								ptr = (void*)&layer.getCamera()->getCamera()->getViewProjection(); // Get the view projection matrix.
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
}