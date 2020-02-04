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

		return model;
	}
}