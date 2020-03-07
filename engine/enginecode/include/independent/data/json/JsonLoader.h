#pragma once

/*! \file JsonLoader.h
\brief Implementation of a loader for json files
*/

#include <json.hpp>
#include <fstream>
#include <future>

#include "JsonLog.h"
#include "JsonLayer.h"
#include "imgui/ImGuiLayer.h"
#include "JsonModelLoader.h"
#include "data/assimp/AssimpModel.h"
#include "rendering/TextLabel.h"

namespace Engine
{
	/*! \class JsonLoader
	\brief Allows to create layers from json files
	*/
	class JsonLoader
	{
	private:
		static HDC hdc; //!< Current device context (windows)
		static HGLRC glrc; //!< Current graphics context (windows)
	public:
		//! Loads json models asynchronously \param filepath path to the model file
		static std::shared_ptr<JsonModel> loadJsonModelAsync(std::string filepath);
		//! Loads assimp models asynchronously \param filepath path to the model file
		static std::shared_ptr<AssimpModel> loadAssimpModelAsync(std::string filepath);
		//! Loads shaders asynchronously \param filepath path to the model file
		static std::shared_ptr<Shader> loadShaderAsync(std::string path);
		//! Loads textures asynchronously \param filepath path to the model file
		static std::shared_ptr<Texture> loadTextureAsync(std::string path);
		//! Loads scenes \param filepath path to the json file \param layer layer to be populated
		static void loadJson(const std::string& filepath, JsonLayer& layer);
		//! Loads GameObjects in the scene \param filepath path to the json file \param layer layer to be populated
		static void loadGameObjects(const std::string& filepath, JsonLayer& layer);
		//! Loads ImGui (for the level editor) \param filepath path to the json file \param layer layer to be populated
		static void loadImGui(const std::string& filepath, ImGuiLayer& layer);
	};
}