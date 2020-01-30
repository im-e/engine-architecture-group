#pragma once

/*! \file JsonLoader.h
\brief Implementation of a loader for json files
*/

#include <json.hpp>
#include <fstream>
#include <future>

#include "JsonLog.h"
#include "JsonLayer.h"
#include "JsonModelLoader.h"
#include "data/assimp/AssimpModel.h"

namespace Engine
{
	/*! \class JsonLoader
	\brief Allows to create layers from json files
	*/
	class JsonLoader
	{
	private:
		static HDC hdc;
		static HGLRC glrc;
	public:
		static JsonModel loadJsonModelAsync(std::string filepath);
		static AssimpModel loadAssimpModelAsync(std::string filepath);
		static std::shared_ptr<Shader> loadShaderAsync(std::string path);
		static void loadJson(const std::string& filepath, JsonLayer& layer);
	};
}