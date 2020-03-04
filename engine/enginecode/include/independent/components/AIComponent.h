#pragma once

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
		std::string m_aiType;
		std::string m_scriptName;
	public:
		AIComponent(float stop, std::string type, std::string scriptName)
			: m_stopSize(stop), m_aiType(type), m_scriptName(scriptName)
		{};

		void onAttach(GameObject* owner) override 
		{ 
			m_owner = owner;		

			luaUpdate();

			m_currentPosition = owner->getComponent<PositionComponent>()->getCurrentPosition();
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

					// find orientation
						// calculate acos of the dot product in each component

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

		void registerClass()
		{
			luabridge::getGlobalNamespace(Application::getInstance().getLuaState())
				.beginClass<AIComponent>("AI")
				.addFunction("numWaypoints", &AIComponent::numWaypoints)
				.addFunction("addWaypoint", &AIComponent::addWaypoint)
				.addFunction("addWaypointFront", &AIComponent::addWaypointFront)
				.endClass();
		}

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

		void setStopDist(float dist) { m_stopSize = dist; }

		float getStopDist()
		{
			return m_stopSize;
		}

		void setAiType(std::string type) { m_aiType = type; }
		std::string& getAiType() { return m_aiType; }

		void setScriptName(std::string scr) { m_scriptName = scr; }
		std::string& getScriptName() { return m_scriptName; }

		~AIComponent()
		{
			
		}
	};
}