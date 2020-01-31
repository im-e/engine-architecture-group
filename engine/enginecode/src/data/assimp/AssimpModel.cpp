#include "engine_pch.h"
#include "data/assimp/AssimpModel.h"

#include <glad/glad.h>

namespace Engine
{
	const aiScene* AssimpModelLoader::m_scene = nullptr;

	void Mesh::setupMesh(VertexData vertices, unsigned int indices)
	{
		//vertex position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

		//vertex normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));

		//vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, texCoords));

		//vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, tangent));

		//bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, bitangent));

		//bone weights
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, boneWeights));

		//bone IDs
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_INT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, boneIDs));
		// How to add animation stuff here ???
	}

	AssimpModel * AssimpModelLoader::loadModel(const std::string & filepath)
	{
		AssimpModel * model = new AssimpModel();
		Assimp::Importer importer;

		m_scene = importer.ReadFile(filepath, aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);

		if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode)
		{
			LogError("Could not load: {0}, error: {1}", filepath, importer.GetErrorString());
			return false;
		}

		processNode(m_scene->mRootNode, m_scene, *model);

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

		return model;
	}
}