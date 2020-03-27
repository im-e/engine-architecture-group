#pragma once

/*! \file AssimpModel.h
\brief Contains methods to process models loaded using ASSIMP library

*/

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>
#include <string>
#include "systems/Log.h"

#include "resources/Shader.h"
#include "resources/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	//! Data of each vertex
	struct VertexData
	{
		glm::vec3 position; //!< Position
		glm::vec3 normal; //!< Normal
		glm::vec3 tangent; //!< Tangent
		glm::vec3 bitangent; //!< Bitangent
		glm::vec2 texCoords; //!< Associated texture coordinates
	};

	/*! \class Mesh
	\brief Definition of each submesh in a model
	*/
	class Mesh
	{
	public:
		//! Constructor \param vertexData vertices \param ind indices
		Mesh(std::vector<VertexData> vertexData, std::vector<unsigned int> ind)
			: m_vertices(vertexData), m_indices(ind) {};

		//! Sets the mesh up on GPU \param vertices vertices to bind \param indices indices to bind
		void setupMesh(VertexData vertices, unsigned int indices);

		//! Vertices of a mesh
		std::vector<VertexData> m_vertices;
		//! Indices of a mesh
		std::vector<unsigned int> m_indices;

		//! Shader associated with a mesh
		std::shared_ptr<Shader> m_shader = nullptr;

		//! Texture associated with a mesh
		std::shared_ptr<Texture> m_texture = nullptr;
	};

	/*! \class AssimpModel
	\brief Contains all submeshes of a model
	*/
	class AssimpModel
	{
	public:
		//! Default constructor
		AssimpModel(){};

		//! Submeshes
		std::vector<Mesh> m_meshes;
	};

	/*! \class AssimpModelLoader
	\brief Loads models using assimp library
	*/
	class AssimpModelLoader
	{
	public:
		//! Assimp scene (a model)
		static const aiScene *m_scene;
		//! Loads a model \param filepath path to the model file
		static AssimpModel* loadModel(const std::string& filepath);

		//! Processes nodes \param node assimp node \param scene assimp scene \param model model to be loaded
		static void processNode(aiNode* node, const aiScene* scene, AssimpModel& model)
		{
			std::string parentName = "None";

			if (node->mParent != nullptr)
				parentName = node->mParent->mName.C_Str();

			aiMatrix4x4* transform = &node->mTransformation;

			
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				model.m_meshes.push_back(processMesh(mesh, scene));
			}
			
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene, model);
			}
			
		}

		//! Processes submeshes \param mesh assimp submesh \param scene assimp scene
		static Mesh processMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<VertexData> vertices;
			std::vector<unsigned int> indices;

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				VertexData v;

				glm::vec3 vector;

				//calculate vertex position
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				v.position = vector;

				//calculate vertex normal
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				v.normal = vector;

				//calculate vertex tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				v.tangent = vector;

				//calculate vertex bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				v.bitangent = vector;

				//if mesh is textured, calculate its coordinates
				if (mesh->mTextureCoords[0])
				{
					glm::vec2 vec;

					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					v.texCoords = vec;
				}
				else
				{
					v.texCoords = glm::vec2(0.0f, 0.0f);
				}

				//add vertex
				vertices.push_back(v);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}

			return Mesh(vertices, indices);
		}
	};
}