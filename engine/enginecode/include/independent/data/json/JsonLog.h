#pragma once

/*! \file JsonLog.h
\brief Provides debugging for json loader.
*/

#include <fstream>

#include "json.hpp"
#include "systems/Log.h"

namespace Engine
{
	/*! \class JsonLog
	\brief Provides implementation of logging events occurred when reading json file
	*/
	class JsonLog
	{
	public:
		//! Logs events occurred when reading json file \param filepath path to the json file
		static void loadLog(const std::string& filepath)
		{
			std::fstream file(filepath, std::ios::in);

			if (!file.is_open())
			{
				LogError("Could not open {0}", filepath);
				return;
			}

			nlohmann::json jsonFile;
			try
			{				
				file >> jsonFile;
			}
			catch (std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}


			LogWarn("RESOURCES");
			if (jsonFile.count("Asyncload") > 0)
			{
				std::string asyncLoadStr[3] = { "shaders", "textures", "fonts" };

				for (int i = 0; i < 3; i++)
				{
					for (auto& fnFilepath : jsonFile["Asyncload"][asyncLoadStr[i]])
					{
						LogInfo("\t{0} name: {1}, filepath: {2}", asyncLoadStr[i], fnFilepath["name"].get<std::string>(), fnFilepath["filepath"].get<std::string>());
					}
				}
			}
			else LogInfo("No resources");

			LogWarn("Array sizes for layer");
			if (jsonFile.count("MemoryInfo") > 0)
			{
				std::string arrayStrings[7] = { "gameObjects", "materials", "position", "velocity", "oscillates", "textures", "controllers" };
				for (int i = 0; i < 6; i++)
				{
					if(jsonFile["MemoryInfo"].count(arrayStrings[i].c_str()) > 0)
						LogInfo("\t{0}\t{1}", arrayStrings[i], jsonFile["MemoryInfo"][arrayStrings[i].c_str()].get<int>());
				}
			}
			else LogInfo("No memory info");

			LogWarn("Cameras");
			if (jsonFile.count("Camera") > 0)
			{
				std::string type = jsonFile["Camera"]["type"].get<std::string>();

				if (type.compare("Fps3D") == 0)
				{
					float fov = jsonFile["Camera"]["fov"].get<float>();
					float aspectRatio = jsonFile["Camera"]["aspectRatio"].get<float>();
					float nearClip = jsonFile["Camera"]["nearClip"].get<float>();
					float farClip = jsonFile["Camera"]["farClip"].get<float>();
					LogInfo("\tEuler3D camera: FOV {0}, aspect ratio {1}, near clip plane {2}, far clip plane {3}", fov, aspectRatio, nearClip, farClip);
				}
				else if (type.compare("Ortho2D") == 0)
				{
					float top = jsonFile["Camera"]["top"].get<float>();
					float left = jsonFile["Camera"]["left"].get<float>();
					float width = jsonFile["Camera"]["width"].get<float>();
					float height = jsonFile["Camera"]["height"].get<float>();
					LogInfo("\tOrthographic 2D camera: top {0}, left {1}, width {2}, height {3}", top, left, width, height);
				}
			}
			else LogInfo("No cameras");

			LogWarn("Renderer");
			if (jsonFile.count("Renderer") > 0)
			{
				std::string type = jsonFile["Renderer"]["type"].get<std::string>();
				LogInfo("\tRenderer type: {0}", type);
			}
			else LogInfo("No renderer");

			LogWarn("GameObjects");
			if (jsonFile.count("GameObject") > 0)
			{
				for (auto& go : jsonFile["GameObject"])
				{
					LogInfo("{0}", go["name"].get<std::string>());
					if (go.count("material") > 0)
					{
						std::string meshType = go["material"]["type"].get<std::string>();
						
						LogInfo("Type: {0}", meshType);

						LogInfo("Material:");
						if (go["material"].count("model") > 0)
						{
							LogInfo("\tVAO: {0}", go["material"]["model"].get<std::string>());
							LogInfo("\tShader: {0}", go["material"]["shader"].get<std::string>());
						}
						// TODO text
					}
					if (go.count("position") > 0)
					{
						LogInfo("Position components");
						LogInfo("\tTranslation: ({0}, {1}, {2})", go["position"]["translation"]["x"].get<float>(), go["position"]["translation"]["y"].get<float>(), go["position"]["translation"]["z"].get<float>());
						LogInfo("\tRotation: ({0}, {1}, {2})", go["position"]["rotation"]["x"].get<float>(), go["position"]["rotation"]["y"].get<float>(), go["position"]["rotation"]["z"].get<float>());
						LogInfo("\tScale: ({0}, {1}, {2})", go["position"]["scale"]["x"].get<float>(), go["position"]["scale"]["y"].get<float>(), go["position"]["scale"]["z"].get<float>());
					}
					if (go.count("velocity") > 0) 
					{
						LogInfo("Velocity Component");
						LogInfo("\tLinear: ({0}, {1}, {2})", go["velocity"]["linear"]["x"].get<float>(), go["velocity"]["linear"]["y"].get<float>(), go["velocity"]["linear"]["z"].get<float>());
						LogInfo("\tAngular: ({0}, {1}, {2})", go["velocity"]["angular"]["x"].get<float>(), go["velocity"]["angular"]["y"].get<float>(), go["velocity"]["angular"]["z"].get<float>());
					}
					if (go.count("oscillate") > 0) 
					{
						LogInfo("Oscillate Component");
						LogInfo("\tState: {0}", go["oscillate"]["state"].get<std::string>());
						LogInfo("\tCurrent time: {0}", go["oscillate"]["current time"].get<float>());
						LogInfo("\tMax time: {0}", go["oscillate"]["max time"].get<float>());
						LogInfo("\tSet texture: {0}", go["oscillate"]["set texture"].get<bool>());
					}
					if (go.count("texture") > 0)
					{
						LogInfo("Texture component");
						LogInfo("\tTexture: {0}", go["texture"]["name"].get<std::string>());
					}
					if (go.count("controller") > 0)
					{
						LogInfo("Controller component");
						LogInfo("Move speed: {0}", go["controller"]["moveSpeed"].get<float>());
						LogInfo("Rotation speed: {0}", go["controller"]["rotationSpeed"].get<float>());
					}
				}
			}
			else LogInfo("No GameObjects");

			LogWarn("UBOs");
			if (jsonFile.count("UBO") > 0)
			{
				for (auto& ubo : jsonFile["UBO"])
				{
					if (ubo.count("name") > 0)
					{
						LogInfo("UBO name: {0}", ubo["name"].get<std::string>());
					}

					if (ubo.count("layout") > 0)
					{
						LogInfo("Layout {");
						for (auto& lay : ubo["layout"])
						{
							LogInfo("{0}", lay.get<std::string>());
						}
						LogInfo("}");
					}
					if (ubo.count("shaders") > 0)
					{
						LogInfo("Bound to shaders: ");
						for (auto& shader : ubo["shaders"])
						{
							if (shader.count("name") > 0 && shader.count("block") > 0)
							{
								LogInfo("\t{0} block {1}", shader["name"].get<std::string>(), shader["block"].get<std::string>());
							}
						}
					}
					if (ubo.count("data") > 0)
					{
						LogInfo("UBO data: ");
						for (auto& data : ubo["data"])
						{
							auto& type = data["type"];
							if (type == "pointer")
							{
								if (data["var"].get<std::string>().compare("VP Matrix") == 0)
									LogInfo("Camera view-projection matrix");
								if (data["var"].get<std::string>().compare("Camera Position") == 0)
								{
									LogInfo("Camera-position"); // Get the position of the camera.
								}
								else LogInfo("Unknown pointer");
							}
							if (type == "Float3") LogInfo("\tFloat3: ({0}, {1}, {2})", data["x"].get<float>(), data["y"].get<float>(), data["z"].get<float>());
						}
					}

					LogWarn("Render commands");
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

									if (type.compare("ClearDepthColorBuffer") == 0)
									{
										LogInfo("ClearDepthColorBuffer()");
									}

									if (type.compare("ClearColor") == 0)
									{
										float r = stage["r"].get<float>();
										float g = stage["g"].get<float>();
										float b = stage["b"].get<float>();
										float a = stage["a"].get<float>();
										LogInfo("ClearColour({0}, {1}, {2}, {3})", r, g, b, a);
									}

									if (type.compare("DepthTestLess") == 0)
									{
										bool enabled = stage["enabled"].get<bool>();
										LogInfo("DepthTestLess: {0}", enabled);
									}

									if (type.compare("BackfaceCulling") == 0)
									{
										bool enabled = stage["enabled"].get<bool>();
										LogInfo("BackfaceCulling: {0}", enabled);
									}

									if (type.compare("MinusOneAlphaBlending") == 0)
									{
										bool enabled = stage["enabled"].get<bool>();
										LogInfo("MinusOneAlphaBlending: {0}", enabled);
									}

									if (type.compare("Debug") == 0)
									{
										bool enabled = stage["enabled"].get<bool>();
										LogInfo("Debug output: {0}", enabled);
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