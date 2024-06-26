#include "engine_pch.h"
#include "resources/ResourceManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include "rendering/Character.h"
#include <glad/glad.h>

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

		return m_VAOs.getAsset(key);
	}

	std::shared_ptr<VertexBuffer> ResourceManager::addVBO(const std::string & key, float * vertices, unsigned int size, BufferLayout & layout)
	{
		std::shared_ptr<VertexBuffer> vbo;
		vbo.reset(VertexBuffer::create(vertices, size, layout));
		m_VBOs.add(key, vbo);

		return m_VBOs.getAsset(key);
	}

	std::shared_ptr<IndexBuffer> ResourceManager::addEBO(const std::string & key, unsigned int * indices, unsigned int size)
	{
		std::shared_ptr<IndexBuffer> ebo;
		ebo.reset(IndexBuffer::create(indices, size));
		m_EBOs.add(key, ebo);

		return m_EBOs.getAsset(key);
	}

	std::shared_ptr<Shader> ResourceManager::addShader(const std::string & key, const std::string & filepath)
	{
		std::shared_ptr<Shader> shader;
		shader.reset(Shader::create(filepath));
		m_shaders.add(key, shader);

		return m_shaders.getAsset(key);
	}

	void ResourceManager::addShaderAsync(const std::string & key, std::shared_ptr<Shader> shader)
	{
		m_shaders.add(key, shader);
	}

	std::shared_ptr<Shader> ResourceManager::addShader(const std::string & key, const std::string & vertexPath, const std::string & fragmentPath)
	{
		std::shared_ptr<Shader> shader;
		shader.reset(Shader::create(vertexPath, fragmentPath));
		m_shaders.add(key, shader);

		return m_shaders.getAsset(key);
	}

	std::shared_ptr<Texture> ResourceManager::addTexture(const std::string & key, const std::string & filepath)
	{
		std::shared_ptr<Texture> texture;
		texture.reset(Texture::createFromFile(filepath));
		m_textures.add(key, texture);

		return m_textures.getAsset(key);
	}

	void ResourceManager::addTextureAsync(const std::string & key, std::shared_ptr<Texture> tex)
	{
		m_textures.add(key, tex);
	}

	std::shared_ptr<Texture> ResourceManager::addTexture(const std::string & key, unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData)
	{
		std::shared_ptr<Texture> texture;
		texture.reset(Texture::createFromRawData(width, height, channels, texData));
		m_textures.add(key, texture);

		return m_textures.getAsset(key);
	}

	std::shared_ptr<Texture> ResourceManager::addSkybox(const std::string & key, std::vector<std::string> faces)
	{
		std::shared_ptr<Texture> texture;
		////texture.reset(Texture::createSkybox(faces));
		//m_textures.add(key, texture);
		return texture;
	}

	std::shared_ptr<Material> ResourceManager::addMaterial(const std::string & key, const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO)
	{
		std::shared_ptr<Material> material;
		material.reset(Material::create(shader, VAO));
		m_materials.add(key, material);

		return m_materials.getAsset(key);
	}

	std::shared_ptr<Material> ResourceManager::addMaterial(const std::string & key, const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexBuffer>& VBO)
	{
		std::shared_ptr<Material> material;
		material.reset(Material::create(shader, VBO));
		m_materials.add(key, material);

		return m_materials.getAsset(key);
	}

	std::shared_ptr<UniformBuffer> ResourceManager::addUBO(const std::string & key, unsigned int size, UniformLayout & layout)
	{
		std::shared_ptr<UniformBuffer> ubo;
		ubo.reset(UniformBuffer::create(size, layout));
		m_UBOs.add(key, ubo);

		return m_UBOs.getAsset(key);
	}

	std::shared_ptr<VertexArray> ResourceManager::overwriteVAO(const std::string& key)
	{
		std::shared_ptr<VertexArray> vao;
		vao.reset(VertexArray::create());
		m_VAOs.remove(key);
		m_VAOs.add(key, vao);

		return m_VAOs.getAsset(key);
	}

	std::shared_ptr<VertexBuffer> ResourceManager::overwriteVBO(const std::string& key, float* vertices, unsigned int size, BufferLayout& layout)
	{
		std::shared_ptr<VertexBuffer> vbo;
		vbo.reset(VertexBuffer::create(vertices, size, layout));
		m_VBOs.remove(key);
		m_VBOs.add(key, vbo);

		return m_VBOs.getAsset(key);
	}

	std::shared_ptr<IndexBuffer> ResourceManager::overwriteEBO(const std::string& key, unsigned int* indices, unsigned int size)
	{
		std::shared_ptr<IndexBuffer> ebo;
		ebo.reset(IndexBuffer::create(indices, size));
		m_EBOs.remove(key);
		m_EBOs.add(key, ebo);

		return m_EBOs.getAsset(key);
	}

	std::shared_ptr<Material> ResourceManager::overwriteMaterial(const std::string & key, const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO)
	{
		std::shared_ptr<Material> material;
		material.reset(Material::create(shader, VAO));
		m_materials.remove(key);
		m_materials.add(key, material);

		return m_materials.getAsset(key);
	}

	void ResourceManager::populateCharacters(std::unordered_map<std::string, unsigned int> fontsAndSizes)
	{
		unsigned char* texMemory;
		unsigned int memH = 1024;
		unsigned int memW = 1024;
		unsigned int usedX = 0;
		unsigned int usedY = 0;
		unsigned int nextUsedY = 0;
		const unsigned int padding = 1u;

		texMemory = (unsigned char*)malloc(memH * memW);
		memset(texMemory, 0, memH * memW);

		FT_Library ft;
		FT_Face face;

		for (auto it : fontsAndSizes)
		{
			std::string filepath = it.first;
			int charsize = it.second;

			unsigned int maxCharHeight = 0;

			if (FT_Init_FreeType(&ft)) LogError("Error: Could not start FreeType");

			if (FT_New_Face(ft, filepath.c_str(), 0, &face)) LogError("Error: Freetype couldn't load font");

			if (FT_Set_Pixel_Sizes(face, 0, charsize)) LogError("Error: Freetype could not set font face size of {0}", charsize);

			for (int i = m_ASCIIstart; i <= m_ASCIIend; i++)
			{
				if (FT_Load_Char(face, i, FT_LOAD_RENDER)) LogError("Error: Could not load the character");

				m_characters[filepath].push_back(Character(
					glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x,
					glm::vec2(0.0f),
					glm::vec2(1.0f)));

				maxCharHeight = std::max(maxCharHeight, face->glyph->bitmap.rows);
			}

			for (int i = m_ASCIIstart; i <= m_ASCIIend; i++)
			{
				if (FT_Load_Char(face, i, FT_LOAD_RENDER)) LogError("Error: Could not load the character");

				glm::ivec2 charSize = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);

				if (usedX + charSize.x + padding >= memW)
				{
					usedX = 0;
					usedY = nextUsedY + padding;
					nextUsedY = usedY + maxCharHeight;
				}

				nextUsedY = std::max(nextUsedY, usedY + charSize.y);
				for (unsigned int y = 0; y < charSize.y; y++)
				{
					unsigned int yOffsetChar = y * charSize.x;
					unsigned int yOffsetMain = (y + usedY) * memW;

					for (unsigned int x = 0; x < charSize.x; x++)
					{
						unsigned int offsetChar = yOffsetChar + x;
						unsigned int offsetMain = yOffsetMain + (x + usedX);

						*(texMemory + offsetMain) = *(face->glyph->bitmap.buffer + offsetChar);
					}
				}

				glm::vec2 topLeft = glm::vec2((float)usedX / (float)memW, (float)usedY / (float)memH);
				glm::vec2 bottomRight = glm::vec2((float)(usedX + charSize.x) / (float)memW, (float)(usedY + charSize.y) / (float)memH);

				m_characters[filepath][i - m_ASCIIstart].setUVs(topLeft, bottomRight);

				usedX += charSize.x + padding;
			}
		}

		m_fontTexture = ResourceManager::addTexture("fontTexture", memW, memH, 1, texMemory);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	std::shared_ptr<Character> ResourceManager::getCharacter(std::string font, unsigned int ASCIIcode)
	{
		std::shared_ptr<Character> chosenCharacter;

		for (auto& it : m_characters)
		{
			if (it.first == font)
			{
				chosenCharacter = std::make_shared<Character>(it.second[ASCIIcode - m_ASCIIstart]);
				return chosenCharacter;
			}
		}
	}

	std::shared_ptr<Sound> ResourceManager::addSound(const std::string& key, bool b3d, bool bLooping, bool bStream, float minDist, float maxDist, RollOff rollOff)
	{
		std::shared_ptr<Sound> sound;
		sound.reset(Sound::create(key, b3d, bLooping, bStream, minDist, maxDist, rollOff));
		m_sounds.add(key, sound);

		return sound;
	}


	void ResourceManager::addJsonModelAsync(const std::string & key, std::shared_ptr<JsonModel> model)
	{
		m_jsonModels.add(key, model);
	}

	void ResourceManager::addAssimpModelAsync(const std::string & key, std::shared_ptr<AssimpModel> model)
	{
		m_assimpModels.add(key, model);
	}

	AssetManager<VertexArray>& ResourceManager::getVAO()
	{
		return m_VAOs;
	}

	AssetManager<VertexBuffer>& ResourceManager::getVBO()
	{
		return m_VBOs;
	}

	AssetManager<IndexBuffer>& ResourceManager::getEBO()
	{
		return m_EBOs;
	}

	AssetManager<Shader>& ResourceManager::getShader()
	{
		return m_shaders;
	}

	AssetManager<Texture>& ResourceManager::getTexture()
	{
		return m_textures;
	}

	AssetManager<Material>& ResourceManager::getMaterial()
	{
		return m_materials;
	}

	AssetManager<UniformBuffer>& ResourceManager::getUBO()
	{
		return m_UBOs;
	}

	AssetManager<JsonModel>& ResourceManager::getJsonModels()
	{
		return m_jsonModels;
	}

	AssetManager<AssimpModel>& ResourceManager::getAssimpModels()
	{
		return m_assimpModels;
	}

	AssetManager<Sound>& ResourceManager::getSound()
	{
		return m_sounds;
	}

	ResourceManager::~ResourceManager()
	{		
	}
}