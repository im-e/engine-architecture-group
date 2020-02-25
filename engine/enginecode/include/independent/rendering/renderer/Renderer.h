#pragma once

/*! \file Renderer.h
\brief API agnostic implementation of the engine renderer
*/

#include "rendering/Material.h"

namespace Engine
{
	/*! \class RenderCommand
	\brief API agnostic implementation of various commands to be used in the renderer
	*/
	class RenderCommand
	{
	protected:
		bool m_alive = false; //!< Should the command stay alive?
	public:
		virtual void action() = 0; //!< Calls the command 

		static RenderCommand* clearDepthColorBuffer(bool alive); //!< Command for enabling depth (3D view)
		//! Command for setting background color \param r red value \param g green value \param b blue value \param a alpha value
		static RenderCommand* setClearColor(bool alive, float r, float g, float b, float a); 
		//! Command for setting depth test \param enabled if the command should be in effect
		static RenderCommand* setDepthTestLest(bool alive, bool enabled);
		//! Command for setting backface culling \param enabled if the command should be in effect
		static RenderCommand* setBackfaceCulling(bool alive, bool enabled);
		//! Command for setting debug output \param enabled if the command should be in effect
		static RenderCommand* setDebugOutput(bool alive, bool enabled);
		//! Command for setting alpha blending \param enabled if the command should be in effect
		static RenderCommand* setMinusOneAlphaBlending(bool alive, bool enabled);

		//! Is a command still alive? \return true if yes
		inline bool isAlive() { return m_alive; }
	};

	//! Map of scene data
	using SceneData = std::unordered_map<std::shared_ptr<UniformBuffer>, std::vector<void*>>;
	
	/*! \class Renderer
	\brief API agnostic implementation of the renderer
	*/
	class Renderer
	{
	public:
		virtual void actionCommand(RenderCommand* command) = 0; //!< Issues commands
		virtual void beginScene(const SceneData& sceneData) = 0; //!< Uploads common data in the scene (frame)
		virtual void endScene() = 0; //!< Ends scene (frame)
		virtual void submit(const std::shared_ptr<Material>& material) = 0; //!< Submits material for drawing \param material material to be submitted
		virtual void tessSubmit(const std::shared_ptr<Material>& material) = 0;
		virtual void flush() = 0; //!< Draw everything (used with batch rendering)

		//! Creates 2D view for the engine
		static Renderer* create2D();
		//! Creates 3D view for the engine
		static Renderer* create3D();
	}; 
}