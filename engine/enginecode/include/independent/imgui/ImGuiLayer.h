#pragma once

/*! \file ImGuiLayer.h
\brief Handles ImGui layers (in the level editor)
*/

#include "rendering/layers/Layer.h"
#include "data/json/JsonLayer.h"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	private:
		JsonLayer* m_layer; //!< Layer to be modified by the editor

		bool m_gameObjectWindow; //!< Window to manage GOs
		bool m_manageCompWindow; //!< Window to manage components
		bool m_managePPWindow;

		std::string m_name; //! Name of the GameObject
		const std::string& m_filepath; //! Path to the file

		//! Set of functions to be displayed when managing components
		std::vector<std::function<void(JsonLayer*)>> m_functions;
		//! Shader names
		std::vector<std::string> m_shadersNames;
		//! Json model names
		std::vector<std::string> m_jsonModelNames;
		//! Assimp model names
		std::vector<std::string> m_assimpModelNames;
		//! Texture names
		std::vector<std::string> m_texturesNames;

	public:
		//! Constructor \param name layer name \param path filepath to load ImGui from \param layer layer to be modified
		ImGuiLayer(const std::string& name, const std::string& path, std::shared_ptr<Layer> layer) : m_filepath(path), Layer(name)
		{
			m_layer = dynamic_cast<Engine::JsonLayer*>(layer.get());
		};

		// Inherited via Layer
		void onAttach() override;
		void onDetach() override;
		void onUpdate(float timestep) override;
		void onEvent(Event & e) override;

		//! Opens the window to manage GOs
		void addGO();
		//! Opens the window to manage components
		void manageComponents();

		void managePPRenderer();
		//! Adds a function to set of functions \param func new function
		void addImGuiFunction(std::function<void(JsonLayer*)> func);
		//! Gets all the functions \return ImGui functions 
		std::vector<std::function<void(JsonLayer*)>>& getFunctions();

		//! Gets shader names \return all shaders names
		std::vector<std::string>& getShaderNames();
		//! Gets json models names \return all json models names
		std::vector<std::string>& getJsonModelsNames();
		//! Gets assimp models names \return all assimp models names
		std::vector<std::string>& getAssimpModelsNames();
		//! Gets textures names \return all textures names
		std::vector<std::string>& getTexturesNames();

		//! Gets associated json layer \return associated json layer
		JsonLayer* getJsonLayer();
		//! Gets name of a currently selected GO \return name of a GO
		std::string& getGOName();
	};
}