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

/* STEPS 

	????????????????????????????????????????????? HOW TO ?????????????????????????????????????????????

	1. Modify code to set vertex data rather than bone data & add number of bones
	2. Create shader (look up lecture)
	3. Test if works --> run and see the T-pose
	4. Create animator component
		4a. Contains 3 vectors (position, rotation, scale keyframes)
		4b. Write onUpdate so it linearly interpolates between keyframes in above vectors (look up lecture for formulas)
		4c. Add extra component messages
		4d. Calculate bone models transforms (array of number of bones) in animator (look up lecture)
		4e. Upload transforms to the shader

	ALSO:
	- what is needed for Lua?
	- how to make finding and compiling user scripts dynamic (without hardcoding scripts stacks or sth)?
	- how to make various fucntions (e.g) onUpdate accessible and modifiable from Lua?
	- how to make Lua scripts components?
*/

namespace Engine
{
	struct VertexData
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec2 texCoords;
		float boneWeights[4] = { 0.f, 0.f, 0.f, 0.f };
		unsigned int boneIDs[4] = { 0, 0, 0, 0 };
	};

	class Mesh
	{
	public:
		Mesh(std::vector<VertexData> vertexData, std::vector<unsigned int> ind, unsigned int bones)
			: m_vertices(vertexData), m_indices(ind), m_numBones(bones) {};

		void setupMesh(VertexData vertices, unsigned int indices);
		std::vector<VertexData> m_vertices;
		std::vector<unsigned int> m_indices;
		unsigned int m_numBones;

		std::shared_ptr<Shader> m_shader = nullptr;
		std::shared_ptr<Texture> m_texture = nullptr;
	};

	class AssimpModel
	{
	public:
		AssimpModel() : m_numBones(0) {};
		std::vector<Mesh> m_meshes;
		std::vector<aiVectorKey> m_positionKeys;
		std::vector<aiQuatKey> m_rotationKeys;
		std::vector<aiVectorKey> m_scaleKeys;

		unsigned int m_numBones;
	};

	class AssimpModelLoader
	{
	public:
		static const aiScene *m_scene;
		static AssimpModel* loadModel(const std::string& filepath);

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
				model.m_numBones = mesh->mNumBones;
			}
			
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene, model);
			}
			
		}

		static Mesh processMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::multimap<unsigned int, std::pair<unsigned int, float>> vertexBoneWeights;

			int bones;
			std::vector<VertexData> vertices;
			std::vector<unsigned int> indices;

			for (unsigned int i = 0; i < mesh->mNumBones; i++) // ???
			{
				aiBone* bone = mesh->mBones[i];
				aiMatrix4x4 transform = bone->mOffsetMatrix;
				bones++;

				for (int j = 0; j < bone->mNumWeights; j++)
				{
					//LogWarn("Bone idx: {0} VertexID: {1} Weight: {2}", i, bone->mWeights[j].mVertexId, bone->mWeights[j].mWeight);
					vertexBoneWeights.insert(std::pair<unsigned int, std::pair<unsigned int, float>>
						(bone->mWeights[j].mVertexId, std::pair<unsigned int, float>(i, bone->mWeights[j].mWeight)));
				}
			}

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

				auto boneData = vertexBoneWeights.equal_range(i); // ???
				int j = 0;
				for (auto it = boneData.first; it != boneData.second; ++it)
				{
					if (j > 4)
						LogError("More than 4 bones influence a vertex");
					auto pair = it->second;
					v.boneIDs[j] = pair.first;
					v.boneWeights[j] = pair.second;
					j++;
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

			return Mesh(vertices, indices, bones);
		}
	};
}