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

		// How to add animation stuff here ???
	}
}