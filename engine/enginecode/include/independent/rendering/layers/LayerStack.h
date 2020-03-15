#pragma once

/*! \file LayerStack.h
\brief Implementation of a stack of layers (e.g UI, game layer) in a game engine
*/

#include "systems/system.h"
#include "Layer.h"
#include <memory>

namespace Engine
{
	/*! \class LayerStack
	\brief Definition of a stack of layers
	
		Inherits from System
	*/
	class LayerStack : public System
	{
	private:
		std::vector<std::shared_ptr<Layer>> m_layers; //!< Collection of all layers

	public:
		//! Default constructor
		LayerStack() {};

		// Inherited via System
		virtual void start(SystemSignal init, ...) override;
		virtual void stop(SystemSignal close, ...) override;

		//! Adds a layer \param layer layer to be added
		void push(std::shared_ptr<Layer> layer);
		//! Removes a layer \param layer layer to be removed
		void pop(std::shared_ptr<Layer> layer);

		//! Gets entire collection of layers \return collection of layers
		inline std::vector<std::shared_ptr<Layer>> getLayers() { return m_layers; }
		//! Gets beginning of a collection of layers \return beginning of a collection
		inline std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_layers.begin(); }
		//! Gets end of a collection of layers \return end of a collection
		inline std::vector<std::shared_ptr<Layer>>::iterator end() { return m_layers.end(); }
	};
}