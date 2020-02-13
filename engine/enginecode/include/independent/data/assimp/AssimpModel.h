#pragma once

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
	struct VertexData
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec2 texCoords;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<VertexData> vertexData, std::vector<unsigned int> ind)
			: m_vertices(vertexData), m_indices(ind) {};

		void setupMesh(VertexData vertices, unsigned int indices);

		std::vector<VertexData> m_vertices;
		std::vector<unsigned int> m_indices;

		std::shared_ptr<Shader> m_shader = nullptr;
		std::shared_ptr<Texture> m_texture = nullptr;
	};

	class AssimpModel
	{
	public:
		AssimpModel() {};
		std::vector<Mesh> m_meshes;
	};

	class AssimpModelLoader
	{
	public:
		static bool loadModel(const std::string& filepath, AssimpModel& model)
		{
			Assimp::Importer importer;

			const aiScene *scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				LogError("Could not load: {0}, error: {1}", filepath, importer.GetErrorString());
				return false;
			}

			processNode(scene->mRootNode, scene, model);

			return true;
		}

		static void processNode(aiNode* node, const aiScene* scene, AssimpModel& model)
		{
			std::string parentName = "None";

			if (node->mParent != nullptr)
				parentName = node->mParent->mName.C_Str();

#ifdef NG_DEBUG

			if (node->mNumMeshes == 0)
				LogInfo("Unmeshed mode: {0}, parent: {1}", node->mName.C_Str(), parentName);

			else
				LogInfo("Meshed mode: {0}, parent: {1}", node->mName.C_Str(), parentName);
#endif // NG_DEBUG

			aiMatrix4x4* transform = &node->mTransformation;

#ifdef NG_DEBUG
			LogWarn("Transform");
			LogInfo("{0} {1} {2} {3}", transform->a1, transform->a2, transform->a3, transform->a4);
			LogInfo("{0} {1} {2} {3}", transform->b1, transform->b2, transform->b3, transform->b4);
			LogInfo("{0} {1} {2} {3}", transform->c1, transform->c2, transform->c3, transform->c4);
			LogInfo("{0} {1} {2} {3}", transform->d1, transform->d2, transform->d3, transform->d4);
#endif // NG_DEBUG

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