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

/* QUESTIONS:

	- Issue with test config???
	- Is multithreading alright???
	- How to finish the animation??? (get all necessary data and pass it to animator component)
	- How to create a shader to take care of animation (can I just modify texturedPhong.glsl)?
	- How to create animator component???
	- How to get and extract keyframes (to call an animation, or to possibly swap them later in the runtime)???

*/

/* STEPS
	1. Modify code to set vertex data rather than bone data & add number of bones
	2. Create shader (look up lecture)
	3. Test if works --> run and see the T-pose
	4. Create animator component
		4a. Contains 3 data structures (position, rotation, scale keyframes)
		4b. Write onUpdate so it linearly interpolates between keyframes (look up lecture for formulas)
		4c. Add extra component messages
		4d. Calculate bone models transforms (array of number of bones) in animator
		4e. Upload transforms to the shader
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


	struct BoneData
	{
		
		//float m_weights[4];
	};

	class Mesh
	{
	public:
		Mesh(std::vector<VertexData> vertexData, std::vector<unsigned int> ind, std::vector<BoneData> bones)
			: m_vertices(vertexData), m_indices(ind), m_bones(bones) {};

		void setupMesh(VertexData vertices, unsigned int indices);
		std::vector<VertexData> m_vertices;
		std::vector<BoneData> m_bones;
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
		static const aiScene *m_scene;
		static bool loadModel(const std::string& filepath, AssimpModel& model)
		{
			Assimp::Importer importer;

			m_scene = importer.ReadFile(filepath, aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);

			if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode)
			{
				LogError("Could not load: {0}, error: {1}", filepath, importer.GetErrorString());
				return false;
			}

			processNode(m_scene->mRootNode, m_scene, model);

#ifdef NG_DEBUG
			LogWarn("Animations");
#endif
			for (int i = 0; i < m_scene->mNumAnimations; i++) // ???
			{
				aiAnimation* animation = m_scene->mAnimations[i];
#ifdef NG_DEBUG
				//LogInfo("Ticks per second: {0}, duration: {1}", animation->mTicksPerSecond, animation->mDuration);
#endif
				for (int j = 0; j < animation->mNumChannels; j++)
				{
					aiNodeAnim* nodeAnim = animation->mChannels[j];

#ifdef NG_DEBUG
					//LogInfo("Bone: {0}", nodeAnim->mNodeName.C_Str());
					//LogWarn("Position keyframes");
#endif
					for (int k = 0; k < nodeAnim->mNumPositionKeys; k++)
					{
						aiVectorKey key = nodeAnim->mPositionKeys[k];
#ifdef NG_DEBUG
						//LogInfo("Time: {0}, Position: {1}, {2}, {3}", key.mTime, key.mValue.x, key.mValue.y, key.mValue.z);
#endif
					}

#ifdef NG_DEBUG
					//LogWarn("Rotation keyframes");
#endif
					for (int k = 0; k < nodeAnim->mNumRotationKeys; k++)
					{
						aiQuatKey key = nodeAnim->mRotationKeys[k];
#ifdef NG_DEBUG
						//LogInfo("Time: {0}, Position: {1}, {2}, {3}, {4}", key.mTime, key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
#endif
					}

#ifdef NG_DEBUG
					//LogWarn("Scaling keyframes");
#endif
					for (int k = 0; k < nodeAnim->mNumScalingKeys; k++)
					{
						aiVectorKey key = nodeAnim->mScalingKeys[k];
#ifdef NG_DEBUG
						//LogInfo("Time: {0}, Position: {1}, {2}, {3}", key.mTime, key.mValue.x, key.mValue.y, key.mValue.z);
#endif
					}
				}

			}

			return true;
		}

		static void processNode(aiNode* node, const aiScene* scene, AssimpModel& model)
		{
			std::string parentName = "None";

			if (node->mParent != nullptr)
				parentName = node->mParent->mName.C_Str();

#ifdef NG_DEBUG

			if (node->mNumMeshes == 0)
				//LogInfo("Unmeshed mode: {0}, parent: {1}", node->mName.C_Str(), parentName);

			//else
				//LogInfo("Meshed mode: {0}, parent: {1}", node->mName.C_Str(), parentName);
#endif // NG_DEBUG

			aiMatrix4x4* transform = &node->mTransformation;

#ifdef NG_DEBUG
			//LogWarn("Transform");
			//LogInfo("{0} {1} {2} {3}", transform->a1, transform->a2, transform->a3, transform->a4);
			//LogInfo("{0} {1} {2} {3}", transform->b1, transform->b2, transform->b3, transform->b4);
			//LogInfo("{0} {1} {2} {3}", transform->c1, transform->c2, transform->c3, transform->c4);
			//LogInfo("{0} {1} {2} {3}", transform->d1, transform->d2, transform->d3, transform->d4);
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
			std::multimap<unsigned int, std::pair<unsigned int, float>> vertexBoneWeights;

			std::vector<VertexData> vertices;
			std::vector<BoneData> bones;
			std::vector<unsigned int> indices;

			for (unsigned int i = 0; i < mesh->mNumBones; i++) // ???
			{
				aiBone* bone = mesh->mBones[i];
				aiMatrix4x4 transform = bone->mOffsetMatrix;

#ifdef NG_DEBUG
				//LogWarn("BONE TRANSFORM: {0} Name: {1}", i, bone->mName.C_Str());
				//LogInfo("{0} {1} {2} {3}", transform.a1, transform.a2, transform.a3, transform.a4);
				//LogInfo("{0} {1} {2} {3}", transform.b1, transform.b2, transform.b3, transform.b4);
				//LogInfo("{0} {1} {2} {3}", transform.c1, transform.c2, transform.c3, transform.c4);
				//LogInfo("{0} {1} {2} {3}", transform.d1, transform.d2, transform.d3, transform.d4);
#endif
				for (int j = 0; j < bone->mNumWeights; j++)
				{
				
#ifdef NG_DEBUG
					//LogWarn("Bone idx: {0} VertexID: {1} Weight: {2}", i, bone->mWeights[j].mVertexId, bone->mWeights[j].mWeight);
#endif
					vertexBoneWeights.insert(std::pair<unsigned int, std::pair<unsigned int, float>>
						(bone->mWeights[j].mVertexId, std::pair<unsigned int, float>(i, bone->mWeights[j].mWeight)));
				}
			}

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				VertexData v;
				BoneData b;

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
					bones.push_back(b);
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