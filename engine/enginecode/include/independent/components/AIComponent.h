#pragma once

extern "C"
{
#include "lua.h" 
#include "lualib.h"
#include "lauxlib.h"
}

#include <../LuaBridge/Source/LuaBridge/LuaBridge.h>

#include "Component.h"
#include "systems/Log.h"

#include <deque>

namespace Engine
{
	class AIComponent : public Component
	{
	private:
		std::deque<glm::vec3> m_waypoints;
		std::shared_ptr<luabridge::LuaRef> m_lua;

		glm::vec3 m_currentPosition;
		glm::vec3 m_desiredPosition;

		glm::vec3 m_currentRotation;
		glm::vec3 m_desiredRotation;

		float m_stopSize;
	public:
		AIComponent(float stop) : m_stopSize(stop) {};

		void onAttach(GameObject* owner) override 
		{ 
			m_owner = owner; 
		}

		GameObject* getOwner() override { return m_owner; }

		void onDetach() override {};

		void onUpdate(float timestep) override 
		{
			if ((m_desiredPosition - m_currentPosition).length() < m_stopSize)
			{
				if (m_waypoints.empty() == false)
				{
					m_waypoints.pop_front();
				}

				luaUpdate(); // check for lua stuff

				if (m_waypoints.empty() == false)
				{
					m_desiredPosition = m_waypoints.front();

					// find orientation
						// calculate acos of the dot product in each component
				}
				else 
				{
					m_desiredPosition = m_currentPosition;
					m_desiredRotation = m_currentRotation;
				}
			}

			// Send messages to other (velocity/rigidbody if physics implemented) components
		};

		void onEvent(Event& e) override {};

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		void addWaypoint(float x, float y, float z)
		{
			m_waypoints.push_back(glm::vec3(x, y, z));
		}

		void addWaypointFront(float x, float y, float z)
		{
			m_waypoints.push_front(glm::vec3(x, y, z));
		}

		int numWaypoints() { return m_waypoints.size(); }

		void setLuaFunction(const luabridge::LuaRef& ref)
		{
			m_lua = std::make_shared<luabridge::LuaRef>(ref);
		}
		
		void luaUpdate()
		{
			if (m_lua)
			{
				try
				{
					(*m_lua)(this);
				}
				catch(luabridge::LuaException const& e)
				{
					LogError("LUA EXCEPTION");
					std::cout << e.what() << std::endl;
				}
			}
		}
	};
}