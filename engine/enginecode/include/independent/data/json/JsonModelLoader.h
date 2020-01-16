#pragma once

/*! \file JsonModelLoader.h
\brief Responsible for loading models using .txt files and jsons
*/

#include <string>
#include <fstream>
#include <sstream>

#include "resources/Shader.h"
#include "resources/Texture.h"
#include "resources/ResourceManager.h"

namespace Engine
{
	/*! \class JsonModel
	\brief Definition of a model to be loaded
	*/
	class JsonModel
	{
	public:
		//! Custom constructor
		JsonModel() : vertices(nullptr), indices(nullptr), verticesSize(0), indicesSize(0), shader(nullptr), texture(nullptr) {}
		//! Custom destructor
		~JsonModel() {}
		float * vertices; //!< Array of vertices 
		unsigned int* indices; //!< Array of indices
		unsigned int verticesSize; //!< Number of vertices
		unsigned int indicesSize; //!< Number of indices
		std::shared_ptr<Shader> shader; //!< Shader of a model 
		std::shared_ptr<Texture> texture; //!< Texture of a model
	};

	/*! \class JsonModelLoader
	\brief Provides implementation for loading models
	*/
	class JsonModelLoader
	{
	public:
		//! Loads a model \param filepath path to the file \param model model to be loaded \return true if successful
		static bool loadModel(const std::string& filepath, JsonModel& model)
		{
			std::fstream handle(filepath, std::ios::in);
			if (!handle.is_open()) return false;

			std::string line;
			std::stringstream ss;

			// Read vertex size
			getline(handle, line);
			ss.str(line);
			if (!ss.eof())
			{
				ss >> model.verticesSize;
				if (model.verticesSize <= 0) return false;
			}
			else return false;

			// Set up vertex array
			model.vertices = new float[model.verticesSize];

			// read vertex data
			getline(handle, line);
			float value;

			ss.clear();
			ss.str(line);
			int i = 0;
			while (!ss.eof())
			{
				ss >> value;
				model.vertices[i] = value;
				i++;
			}

			// Read indices size
			getline(handle, line);
			ss.clear();
			ss.str(line);
			if (!ss.eof())
			{
				ss >> model.indicesSize;
				if (model.indicesSize <= 0) return false;
			}
			else
			{
				return false;
			}

			// Set up array
			model.indices = new unsigned int[model.indicesSize];

			// read data
			getline(handle, line);
			unsigned int value2;

			ss.clear();
			ss.str(line);
			i = 0;
			while (!ss.eof())
			{
				ss >> value2;
				model.indices[i] = value2;
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
				model.shader = ResourceManagerInstance->getShader().getAsset(name);
			}
			else model.shader = nullptr;

			// read texture
			getline(handle, line);
			ss.clear();
			ss.str(line);
			if (line.compare("NULL") != 0)
			{
				std::string name;
				ss >> name;
				model.texture = ResourceManagerInstance->getTexture().getAsset(name);
			}
			else model.texture = nullptr;

			return true;
		}
	};
}