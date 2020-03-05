#pragma once

/*! \file JsonLayer.h
\brief Implements layers for the engine loaded using json files
*/

#include "rendering/layers/Layer.h"

#include "components/GameObject.h"
#include "components/MaterialComponent.h"
#include "components/PositionComponent.h"
#include "components/VelocityComponent.h"
#include "components/OscillateComponent.h"
#include "components/TextureComponent.h"
#include "components/ControllerComponent.h"
#include "components/ColourComponent.h"

namespace Engine
{

	/*! \class JsonLayer
	\brief Defines an engine layer
	
		Inherits from Layer
	*/
	class JsonLayer : public Layer
	{
	protected:
		std::vector<std::shared_ptr<Engine::GameObject>> m_gameObjects;			//!< Game objects which belong to the layer
		std::vector<std::shared_ptr<Engine::MaterialComponent>> m_materials;	//!< Material components
		std::vector<std::shared_ptr<Engine::PositionComponent>> m_positions;	//!< Position components
		std::vector<std::shared_ptr<Engine::VelocityComponent>> m_velocities;	//!< Velocity components
		std::vector<std::shared_ptr<Engine::OscillateComponent>> m_oscillates;	//!< Oscillate components
		std::vector<std::shared_ptr<Engine::TextureComponent>> m_textures;		//!< Texture components
		std::vector<std::shared_ptr<Engine::ControllerComponent>> m_controllers; //!< Controller components
		std::vector<std::shared_ptr<Engine::ColourComponent>> m_colours; //!< Colour components

		std::vector<std::shared_ptr<Engine::RenderCommand>> m_initCommands;						//!< Render commands issued on renderer start
		std::vector<std::shared_ptr<Engine::RenderCommand>> m_predrawCommands;					//!< Render commands issued before materials are drawn
		std::vector<std::shared_ptr<Engine::RenderCommand>> m_postdrawCommands;					//!< Render commands issued after materials are drawn
		std::vector<std::shared_ptr<Engine::RenderCommand>> m_exitCommands;						//!< Render commands issued prior to renderer shutdown

		std::list<void*> m_data;			//!< Misc data
		SceneData m_sceneData;				//!< SceneData
		std::string m_filepath;				//!< JSON filepath

	public:
		//! Custom constructor \param filepath path to the json file \param name name of the layer
		JsonLayer(const std::string& filepath, const std::string& name) : m_filepath(filepath), Layer(name) {}	

		//! Runs when layer is attached
		void onAttach() override;
		//! Runs when layer is detached
		void onDetach() override;
		//! Runs every frame
		void onUpdate(float timestep) override;
		//! Runs when an event occurs
		void onEvent(Event& e) override;

		//! Gets a collection of GOs in a layer \return collection of GOs
		inline std::vector<std::shared_ptr<Engine::GameObject>>& getGameObjects() { return m_gameObjects; }
		//! Gets a collection of material components in a layer \return collection of material components
		inline std::vector<std::shared_ptr<Engine::MaterialComponent>>& getMaterials() { return m_materials; }
		//! Gets a collection of position components in a layer \return collection of position components
		inline std::vector<std::shared_ptr<Engine::PositionComponent>>& getPositions() { return m_positions; }
		//! Gets a collection of velocity components in a layer \return collection of velocity components
		inline std::vector<std::shared_ptr<Engine::VelocityComponent>>& getVelocities() { return m_velocities; }
		//! Gets a collection of oscillate components in a layer \return collection of oscillate components
		inline std::vector<std::shared_ptr<Engine::OscillateComponent>>& getOscillates() { return m_oscillates; }
		//! Gets a collection of texture components in a layer \return collection of texture components
		inline std::vector<std::shared_ptr<Engine::TextureComponent>>& getTextures() { return m_textures; }
		//! Gets a collection of controller components in a layer \return collection of controller components
		inline std::vector<std::shared_ptr<Engine::ControllerComponent>>& getControllers() { return m_controllers; }
		//! Gets a collection of colour components in a layer \return collection of colour components
		inline std::vector<std::shared_ptr<Engine::ColourComponent>>& getColours() { return m_colours; }

		//! Gets a collection of predraw commands applied to a layer \return collection of predraw commands
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getPredrawCommands() { return m_predrawCommands; }
		//! Gets a collection of postdraw commands applied to a layer \return collection of postdraw commands
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getPostdrawCommands() { return m_postdrawCommands; }
		//! Gets a collection of init commands applied to a layer \return collection of init commands
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getInitCommands() { return m_initCommands; }
		//! Gets a collection of exit commands applied to a layer \return collection of exit commands
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getExitCommands() { return m_exitCommands; }

		//! Gets UBO data associated with a scene \return scene UBO data
		inline SceneData& getSceneData() { return m_sceneData; }
		//! Gets data associated with a scene \return scene data
		inline std::list<void*>& getJsonData() { return m_data; }
	};
}