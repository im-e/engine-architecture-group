#include "engine_pch.h"
#include "resources/ResourceManager.h"

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

	void ResourceManager::addShaderAsync(const std::string & key, std::shared_ptr<Shader> shader)
	{
		m_shaders.add(key, shader);
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

	void ResourceManager::addTextureAsync(const std::string & key, std::shared_ptr<Texture> tex)
	{
		m_textures.add(key, tex);
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