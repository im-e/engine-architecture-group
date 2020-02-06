#include "engine_pch.h"
#include "resources/ResourceManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include "rendering/Character.h"

namespace Engine
{
	ResourceManager* ResourceManager::s_instance = nullptr;

	ResourceManager::ResourceManager()
	{
		if (s_instance == nullptr)
			s_instance = this;
		else
			getInstance();
	}

	std::shared_ptr<VertexArray> ResourceManager::addVAO(const std::string & key)
	{
		std::shared_ptr<VertexArray> vao;
		vao.reset(VertexArray::create());
		m_VAOs.add(key, vao);

		return vao;
	}

	std::shared_ptr<VertexBuffer> ResourceManager::addVBO(const std::string & key, float * vertices, unsigned int size, BufferLayout & layout)
	{
		std::shared_ptr<VertexBuffer> vbo;
		vbo.reset(VertexBuffer::create(vertices, size, layout));
		m_VBOs.add(key, vbo);

		return vbo;
	}

	std::shared_ptr<IndexBuffer> ResourceManager::addEBO(const std::string & key, unsigned int * indices, unsigned int size)
	{
		std::shared_ptr<IndexBuffer> ebo;
		ebo.reset(IndexBuffer::create(indices, size));
		m_EBOs.add(key, ebo);

		return ebo;
	}

	std::shared_ptr<Shader> ResourceManager::addShader(const std::string & key, const std::string & filepath)
	{
		std::shared_ptr<Shader> shader;
		shader.reset(Shader::create(filepath));
		m_shaders.add(key, shader);

		return shader;
	}

	std::shared_ptr<Shader> ResourceManager::addShader(const std::string & key, const std::string & vertexPath, const std::string & fragmentPath)
	{
		std::shared_ptr<Shader> shader;
		shader.reset(Shader::create(vertexPath, fragmentPath));
		m_shaders.add(key, shader);

		return shader;
	}

	std::shared_ptr<Texture> ResourceManager::addTexture(const std::string & key, const std::string & filepath)
	{
		std::shared_ptr<Texture> texture;
		texture.reset(Texture::createFromFile(filepath));
		m_textures.add(key, texture);

		return texture;
	}

	std::shared_ptr<Texture> ResourceManager::addTexture(const std::string & key, unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData)
	{
		std::shared_ptr<Texture> texture;
		texture.reset(Texture::createFromRawData(width, height, channels, texData));
		m_textures.add(key, texture);

		return texture;
	}

	std::shared_ptr<Material> ResourceManager::addMaterial(const std::string & key, const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO)
	{
		std::shared_ptr<Material> material;
		material.reset(Material::create(shader, VAO));
		m_materials.add(key, material);

		return material;
	}

	std::shared_ptr<Material> ResourceManager::addMaterial(const std::string & key, const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexBuffer>& VBO)
	{
		std::shared_ptr<Material> material;
		material.reset(Material::create(shader, VBO));
		m_materials.add(key, material);

		return material;
	}

	std::shared_ptr<UniformBuffer> ResourceManager::addUBO(const std::string & key, unsigned int size, UniformLayout & layout)
	{
		std::shared_ptr<UniformBuffer> ubo;
		ubo.reset(UniformBuffer::create(size, layout));
		m_UBOs.add(key, ubo);

		return ubo;
	}

	void ResourceManager::populateCharacters(std::unordered_map<std::string, unsigned int> fontsAndSizes)
	{
		/*FT_Library ft;
		FT_Face face;
		//std::string filepath("assets/fonts/04b_20/04b_20__.ttf");
		//int charsize = 128;

		for (auto it : fontsAndSizes)
		{
			std::string filepath = it.first;
			int charsize = it.second;

			if (FT_Init_FreeType(&ft)) LogError("Error: Could not start FreeType");

			if (FT_New_Face(ft, filepath.c_str(), 0, &face)) LogError("Error: Freetype couldn't load font");

			if (FT_Set_Pixel_Sizes(face, 0, charsize)) LogError("Error: Freetype could not set font face size of {0}", charsize);

			for (int i = m_ASCIIstart; i < m_ASCIIend; i++)
			{
				if (FT_Load_Char(face, i, FT_LOAD_RENDER)) LogError("Error: Could not load the character");

				//addTexture("", face->glyph->bitmap.width, face->glyph->bitmap.rows, 1, face->glyph->bitmap.buffer);
				//Texture::createFromRawData(face->glyph->bitmap.width, face->glyph->bitmap.rows, 1, face->glyph->bitmap.buffer);
				m_characters[""].push_back(Character(glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x, glm::vec2(0.0f), glm::vec2(1.0f)));
			}
		}

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char* texMemory;
		int memH = 1024;
		int memW = 1024;
		int usedX = 0;
		int usedY = 0;

		texMemory = (unsigned char*)malloc(memH * memW);
		memset(texMemory, 0, memH * memW); */
	}

	AssetManager<VertexArray> ResourceManager::getVAO()
	{
		return m_VAOs;
	}

	AssetManager<VertexBuffer> ResourceManager::getVBO()
	{
		return m_VBOs;
	}

	AssetManager<IndexBuffer> ResourceManager::getEBO()
	{
		return m_EBOs;
	}

	AssetManager<Shader> ResourceManager::getShader()
	{
		return m_shaders;
	}

	AssetManager<Texture> ResourceManager::getTexture()
	{
		return m_textures;
	}

	AssetManager<Material> ResourceManager::getMaterial()
	{
		return m_materials;
	}

	AssetManager<UniformBuffer> ResourceManager::getUBO()
	{
		return m_UBOs;
	}

	ResourceManager::~ResourceManager()
	{		
	}
}