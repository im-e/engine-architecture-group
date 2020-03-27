#pragma once

/*! \file AIComponent.h
\brief Implements AI for GameObjects
*/

extern "C"
{
#include "lua.h" 
#include "lualib.h"
#include "lauxlib.h"
}
#include <../LuaBridge/Source/LuaBridge/LuaBridge.h>

#include <deque>
#include "Component.h"
#include "systems/Log.h"
#include "core/application.h"

namespace Engine
{
	/*! \class AIComponent
	\brief Defines an AI component
	*/
	class AIComponent : public Component
	{
	private:
		std::deque<glm::vec3> m_waypoints; //!< All waypoints
		std::shared_ptr<luabridge::LuaRef> m_lua; //!< Reference to Lua

		int m_currentWaypoint; //!< Current waypoint an AI has to go to

		glm::vec3 m_currentPosition; //!< Current AI position
		glm::vec3 m_desiredPosition; //!< Desired AI position

		glm::vec3 m_currentRotation; //!< Current AI rotation 
		glm::vec3 m_desiredRotation; //!< Desired AI rotation

		float m_stopSize; //!< AI's stop distance
		std::string m_aiType; //!< Type of the AI
		std::string m_scriptName; //!< Script AI is executing
	public:
		//! Custom constructor \param stop stop distance \param type AI type \param scriptName script that is being executed
		AIComponent(float stop, std::string type, std::string scriptName)
			: m_stopSize(stop), m_aiType(type), m_scriptName(scriptName)
		{};

		void onAttach(GameObject* owner) override 
		{ 
			m_owner = owner;		

			luaUpdate();

			m_currentPosition = owner->getComponent<PositionComponent>()->getCurrentPosition();
			m_currentRotation = owner->getComponent<PositionComponent>()->getCurrentRotation();

			m_desiredPosition = m_waypoints.front();
		}

		GameObject* getOwner() override { return m_owner; }

		void onDetach() override { m_lua.reset(); };

		void onUpdate(float timestep) override 
		{
			if (glm::distance(m_desiredPosition, m_currentPosition) < m_stopSize)
			{
				if (m_waypoints.empty() == false)
				{
					m_waypoints.pop_front();
				}

				luaUpdate(); // check for lua stuff

				if (m_waypoints.empty() == false)
				{
					m_desiredPosition = m_waypoints.front();
					//m_currentWaypoint = m_waypoints.front();

					// find orientation
						// calculate acos of the dot product in each component

					float angleBetween = glm::dot(m_currentPosition, m_desiredPosition);
					glm::vec3 rotationAxis = glm::cross(m_currentPosition, m_desiredPosition);
					float s = glm::sqrt((1 + angleBetween) * 2);
					float invs = 1 / s;

					glm::quat quaternion = glm::quat(s * 0.5f, glm::vec3(rotationAxis) * invs);
					glm::mat4 rotationMatrix = glm::toMat4(quaternion);


					LogInfo("Going to: {0}, {1}, {2}", m_desiredPosition.x, m_desiredPosition.y, m_desiredPosition.z);
				}
				else 
				{
					m_desiredPosition = m_currentPosition;
					m_desiredRotation = m_currentRotation;
				}			
			}

			// Send messages to other (velocity/rigidbody if physics implemented) components
			ComponentMessage posMsg(ComponentMessageType::AIPositionSet, m_desiredPosition);
			ComponentMessage rotMsg(ComponentMessageType::AIRotationSet, m_desiredRotation);

			sendMessage(posMsg);
			sendMessage(rotMsg);

			m_currentPosition = m_owner->getComponent<PositionComponent>()->getCurrentPosition();
			m_currentRotation = m_owner->getComponent<PositionComponent>()->getCurrentRotation();

		};

		void onEvent(Event& e) override {};

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		//! Adds a waypoint to the back of the queue \param x x coordinate \param y y coordinate \param z z coordinate
		void addWaypoint(float x, float y, float z)
		{
			m_waypoints.push_back(glm::vec3(x, y, z));
		}

		//! Adds a waypoint to the front of the queue \param x x coordinate \param y y coordinate \param z z coordinate
		void addWaypointFront(float x, float y, float z)
		{
			m_waypoints.push_front(glm::vec3(x, y, z));
		}

		//! Gets a waypoint at the index \param index waypoint index in a deque
		glm::vec3 getWaypoint(int index)
		{
			return m_waypoints[index];
		}

		//! How many waypoints are currently stored \return size of waypoint deque
		int numWaypoints() { return m_waypoints.size(); }
		//! Gets current waypoint an AI is going to \return current waypoint
		int getCurrentWaypoint() { return m_currentWaypoint; }
		
		//! Sets lua to be executed \param ref lua reference
		void setLuaFunction(const luabridge::LuaRef& ref)
		{
			m_lua = std::make_shared<luabridge::LuaRef>(ref);
		}
		
		//! Updates AI behaviour based on lua scripts
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

		//! Registers a class to LUA
		void registerClass()
		{
			luabridge::getGlobalNamespace(Application::getInstance().getLuaState())
				.beginClass<AIComponent>("AI")
				.addFunction("numWaypoints", &AIComponent::numWaypoints)
				.addFunction("addWaypoint", &AIComponent::addWaypoint)
				.addFunction("addWaypointFront", &AIComponent::addWaypointFront)
				.addFunction("currentWaypoint", &AIComponent::getCurrentWaypoint)
				.endClass();
		}

		//! Executes script \param luaScriptPath path to the script \param tableName lua table to be executed \param luaFuncName lua function to be executed
		void doFile(std::string luaScriptPath, std::string tableName, std::string luaFuncName)
		{
			if (luaL_dofile(Application::getInstance().getLuaState(), luaScriptPath.c_str()) == 0)
			{
				luabridge::LuaRef table = luabridge::getGlobal(Application::getInstance().getLuaState(), tableName.c_str());

				if (table.isTable())
				{
					if (table[luaFuncName].isFunction())
					{
						this->setLuaFunction(table[luaFuncName]);
					}
				}
			}
			else
			{
				LogError("Couldn't load {0}", luaScriptPath);
			}
		}

		//! Sets stop distance \param dist new stop distance
		void setStopDist(float dist) { m_stopSize = dist; }

		//! Gets stop distance \return current stop distance
		float getStopDist()
		{
			return m_stopSize;
		}

		//! Sets AI type \param type new type
		void setAiType(std::string type) { m_aiType = type; }
		//! Gets current AI type \return AI type
		std::string& getAiType() { return m_aiType; }

		//! Sets script name \param scr new script
		void setScriptName(std::string scr) { m_scriptName = scr; }
		//! Get current script name \return current script associated with this component
		std::string& getScriptName() { return m_scriptName; }

		~AIComponent()
		{
			
		}
	};
}