#pragma once

/*! \file JsonModelLoader.h
\brief Responsible for loading models using .txt files and jsons
*/

#include <string>
#include <fstream>
#include <sstream>

#include "resources/Shader.h"
#include "resources/Texture.h"


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
		static JsonModel* loadModel(const std::string& filepath);
	};
}