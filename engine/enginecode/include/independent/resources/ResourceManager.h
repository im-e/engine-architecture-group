#pragma once

/*! \file ResourceManager.h
\brief Responsible for managing resource in the application
*/

#include "systems/system.h"
#include "AssetManager.h"

#include "systems/Log.h"

#include "rendering/Buffers.h"
#include "resources/Texture.h"
#include "resources\Skybox.h"
#include "resources/Shader.h"
#include "rendering/Material.h"
#include "rendering/Character.h"

#include "data/json/JsonModelLoader.h"
#include "data/assimp/AssimpModel.h"


namespace Engine
{
	/*! \class ResourceManager
	\brief Manages resources in the application runtime. 

		Singleton
	*/
	class ResourceManager
	{
	private:
		AssetManager<VertexArray> m_VAOs; //!< Collection of VAOs
		AssetManager<VertexBuffer> m_VBOs; //!< Collection of VBOs
		AssetManager<IndexBuffer> m_EBOs; //!< Collection of index buffers
		AssetManager<Shader> m_shaders; //!< Collection of shaders
		AssetManager<Texture> m_textures; //!< Collection of textures
		AssetManager<Material> m_materials;//!< Collection of materials
		AssetManager<UniformBuffer> m_UBOs; //!< Collection of UBOs
		AssetManager<JsonModel> m_jsonModels; //!< Collection of json models
		AssetManager<AssimpModel> m_assimpModels; //!< Collection of assimp models

		std::map<std::string, std::vector<Character>> m_characters; //!< Renderable characters
		const int m_ASCIIstart = 32; //!< Beginning of ASCII
		const int m_ASCIIend = 126; //!< End of ASCII
		std::shared_ptr<Texture> m_fontTexture; //!< Texture which stores fonts
		std::vector<std::string> m_fontPaths; //!< Font paths

		ResourceManager(); //!< Default constructor
		static ResourceManager* s_instance; //!< Single instance of a class
	public:
		std::shared_ptr<VertexArray> addVAO(const std::string& key); //!< Adds VAO to the collection of VAOs \param key key associated with a VAO
		/*! Adds VBO to the collection of VBOs 
		\param key key associated with a VBO 
		\param vertices VBO vertices 
		\param size VBO size 
		\param layout BufferLayout of a VBO
		*/
		std::shared_ptr<VertexBuffer> addVBO(const std::string& key, float* vertices, unsigned int size, BufferLayout& layout);
		/*! Adds index buffer to collection of index buffers
		\param key key associated with an index buffer
		\param indices index buffer indices
		\param size index buffer size
		*/
		std::shared_ptr<IndexBuffer> addEBO(const std::string& key, unsigned int* indices, unsigned int size);
		/*! Adds shader to collection of shaders
		\param key key associated with a shader
		\param filepath path to the shader file
		*/
		std::shared_ptr<Shader> addShader(const std::string& key, const std::string& filepath);
		//! Adds a compiled shader to the collection \param key key associated with a shader \param shader shader to be added
		void addShaderAsync(const std::string & key, std::shared_ptr<Shader> shader);
		/*! Adds shader to collection of shaders
		\param key key associated with a shader
		\param vertexPath path to the vertex shader file
		\param fragmentPath path to the fragment shader file
		*/
		std::shared_ptr<Shader> addShader(const std::string& key, const std::string& vertexPath, const std::string& fragmentPath);
		/*! Adds texture to collection of textures
		\param key key associated with a texture
		\param filepath path to the texture file
		*/
		std::shared_ptr<Texture> addTexture(const std::string& key, const std::string& filepath);
		//! Adds a compiled texture to the collection \param key key associated with a texture \param texture texture to be added
		void addTextureAsync(const std::string& key, std::shared_ptr<Texture> tex);
		/*! Adds texture to collection of textures
		\param key key associated with a texture
		\param width width of the texture
		\param height height of the texture
		\param channels channels of the texture
		\param texData pointer to texture data in the memory
		*/
		std::shared_ptr<Texture> addTexture(const std::string& key, unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData);
		/*! Adds material to collection of materials (OpenGL)
		\param key key associated with a material
		\param shader shader used by a material
		\param VAO material's geometry
		*/
		std::shared_ptr<Texture> addSkybox(const std::string& key, std::vector<std::string> faces);

		std::shared_ptr<Material> addMaterial(const std::string& key, const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO);
		/*! Adds material to collection of materials (Non OpenGL)
		\param key key associated with a material
		\param shader shader used by a material
		\param VBO material's geometry
		*/
		std::shared_ptr<Material> addMaterial(const std::string& key, const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexBuffer>& VBO);
		/*! Adds UBO to collection of UBOs
		\param key key associated with a UBO
		\param size size of the UBO layout
		\param layout UniformLayout to be used by the UBO
		*/
		std::shared_ptr<UniformBuffer> addUBO(const std::string& key, unsigned int size, UniformLayout& layout);

		//! Adds a compiled json model to the collection \param key key associated with a model \param model model to be added
		void addJsonModelAsync(const std::string& key, std::shared_ptr<JsonModel> model);
		//! Adds a compiled assimp model to the collection \param key key associated with a model \param model model to be added
		void addAssimpModelAsync(const std::string& key, std::shared_ptr<AssimpModel> model);

		//! Gets class instance \return singleton instance
		static ResourceManager* getInstance()
		{
			if (s_instance == nullptr)
				s_instance = new ResourceManager();

			return s_instance;
		};

		std::shared_ptr<VertexArray> overwriteVAO(const std::string& key);
		std::shared_ptr<VertexBuffer> overwriteVBO(const std::string& key, float* vertices, unsigned int size, BufferLayout& layout);
		std::shared_ptr<IndexBuffer> overwriteEBO(const std::string& key, unsigned int* indices, unsigned int size);
		std::shared_ptr<Material> overwriteMaterial(const std::string& key, const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO);

		AssetManager<VertexArray>& getVAO(); //!< Get collection of the VAOs \return m_VAOs collection of the VAOs
		AssetManager<VertexBuffer>& getVBO(); //!< Get collection of the VBOs \return m_VBOs collection of the VBOs
		AssetManager<IndexBuffer>& getEBO(); //!< Get collection of the index buffers \return m_EBOs collection of index buffers
		AssetManager<Shader>& getShader(); //!< Get collection of shaders \return m_shaders collection of shaders
		AssetManager<Texture>& getTexture(); //!< Get collection of textures \return m_textures collection of textures
		AssetManager<Material>& getMaterial(); //!< Get collection of materials \return m_materials collection of materials
		AssetManager<UniformBuffer>& getUBO(); //!< Get collection of UBOs \return m_UBOs collection of UBOs
		AssetManager<JsonModel>& getJsonModels(); //!< Get collection of json models \return m_JsonModels collection of json models
		AssetManager<AssimpModel>& getAssimpModels(); //!< Get collection of assimp models \return m_AssimpModels collection of assimp models

		void populateCharacters(std::unordered_map<std::string, unsigned int> fontsAndSizes); //!< Preloads the characters
		std::shared_ptr<Character> getCharacter(std::string font, unsigned int ASCIIcode); //!< Gets specified character from specified font \return a character
		inline std::shared_ptr<Texture>& getFontTexture() { return m_fontTexture; } //!< Gets the font texture \return font texture
		std::vector<std::string>& getFontPaths() { return m_fontPaths; } //!< Gets font paths \return all font paths

		~ResourceManager(); //!< Default destructor
	};
}

//! \def ResourceManagerInstance Shorthand for getting resource manager class instance
#define ResourceManagerInstance Engine::ResourceManager::getInstance()