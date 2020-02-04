#include "engine_pch.h"
#include "data/json/JsonModelLoader.h"
#include "resources/ResourceManager.h"

namespace Engine
{
	JsonModel * JsonModelLoader::loadModel(const std::string & filepath)
	{
		JsonModel* model = new JsonModel();
		std::fstream handle(filepath, std::ios::in);
		if (!handle.is_open()) return false;

		std::string line;
		std::stringstream ss;

		// Read vertex size
		getline(handle, line);
		ss.str(line);
		if (!ss.eof())
		{
			ss >> model->verticesSize;
			if (model->verticesSize <= 0)
			{
				return nullptr;
			}
		}
		else return nullptr;

		// Set up vertex array
		model->vertices = new float[model->verticesSize];

		// read vertex data
		getline(handle, line);
		float value;

		ss.clear();
		ss.str(line);
		int i = 0;
		while (!ss.eof())
		{
			ss >> value;
			model->vertices[i] = value;
			i++;
		}

		// Read indices size
		getline(handle, line);
		ss.clear();
		ss.str(line);
		if (!ss.eof())
		{
			ss >> model->indicesSize;
			if (model->indicesSize <= 0)
			{
				return nullptr;
			}
		}
		else
		{
			return nullptr;
		}

		// Set up array
		model->indices = new unsigned int[model->indicesSize];

		// read data
		getline(handle, line);
		unsigned int value2;

		ss.clear();
		ss.str(line);
		i = 0;
		while (!ss.eof())
		{
			ss >> value2;
			model->indices[i] = value2;
			i++;
		}

		// read Shader
		getline(handle, line);
		ss.clear();
		ss.str(line);
		if (line.compare("NULL") != 0)
		{
			std::string name;
			ss >> name;
			model->shader = ResourceManagerInstance->getShader().getAsset(name);
		}
		else model->shader = nullptr;

		// read texture
		getline(handle, line);
		ss.clear();
		ss.str(line);
		if (line.compare("NULL") != 0)
		{
			std::string name;
			ss >> name;
			model->texture = ResourceManagerInstance->getTexture().getAsset(name);
		}
		else model->texture = nullptr;

		return model;
	}
}