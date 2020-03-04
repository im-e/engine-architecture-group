#pragma once

/*! \file GameObject.h
\brief Provides implementation for gameobjects.
*/

#include "Component.h"
#include <memory>
#include <vector>
#include <map>

#include "systems/Log.h"

namespace Engine
{
	/*! \class GameObject
	\brief Base class for GameObjects
	*/
	class GameObject
	{
	protected:
		std::string m_name; //!< GameObject name
		std::vector<std::shared_ptr<Component>> m_components; //!< Collection of components bound to GO

		std::multimap<ComponentMessageType, Component*> m_dispatchMap; //!< Maps message types to components

	public:
		//! Custom constructor \param name name of a GO
		GameObject(std::string name) : m_name(name) {};
		void onUpdate(float timestep); //!< Called every tick \param timestep delta time
		void onEvent(Event& e); //!< Called on event \param e event occured
		void addComponent(const std::shared_ptr<Component>& comp); //!< Adds components \param comp component to be added
		void removeComponent(std::shared_ptr<Component> comp); //!< Removes components \param comp component to be removed

		//! Gets component \return component if exists on a gameobject
		template<typename C>
		inline std::shared_ptr<C> getComponent()
		{
			auto result = nullptr;
			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				if (it->get()->getType().hash_code() == typeid(C).hash_code())
				{
					return std::static_pointer_cast<C>(*it);
				}					
			}

			return result;
		}

		//! Gets beginning of components collection associated with a GO \return beginning of collection
		inline std::vector<std::shared_ptr<Component>>::iterator begin() { return m_components.begin(); }
		//! Gets end of components collection associated with a GO \return end of collection
		inline std::vector<std::shared_ptr<Component>>::iterator end() { return m_components.end(); }
		//! Gets whole collection of components associated with a GO \return collection of components
		inline std::vector<std::shared_ptr<Component>>& getComponents() { return m_components; }

		//! Gets a dispatch map associated with a GO \return dispatch map
		inline std::multimap<ComponentMessageType, Component*>& getMap() { return m_dispatchMap; }
		//! Gets a beginning of a dispatch map associated with a GO \return beginning of a dispatch map
		inline std::multimap<ComponentMessageType, Component*>::iterator beginMap() { return m_dispatchMap.begin(); }
		//! Gets an end of a dispatch map associated with a GO \return end of a dispatch map
		inline std::multimap<ComponentMessageType, Component*>::iterator endMap() { return m_dispatchMap.end(); }

		//! Gets GO's name \return GameObject's name
		inline std::string& getName() { return m_name; }

		//! Virtual destructor
		virtual ~GameObject() 
		{
		};
	};
}