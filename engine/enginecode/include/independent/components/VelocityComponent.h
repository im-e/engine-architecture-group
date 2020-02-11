#pragma once

/*! \file VelocityComponent.h
\brief Implementation of a component responsible for applying velocity to a GO
*/

#include "Component.h"

namespace Engine
{
	/*! \class VelocityComponent
	\brief Implements velocity component for GO. 
	
		Inherits from Component
	*/
	class VelocityComponent : public Component
	{
	private:
		glm::vec3 m_linear; //!< Linear velocity
		glm::vec3 m_angular; //!< Angular velocity
	public:
		//! Custom constructor. \param linear initial linear velocity \param angular initial angular velocity
		VelocityComponent(glm::vec3 linear, glm::vec3 angular)
			: m_linear(linear), m_angular(angular)
		{};

		void onUpdate(float timestep) override
		{
			std::pair<glm::vec3, glm::vec3> data(m_linear * timestep, m_angular * timestep);
			sendMessage(ComponentMessage(ComponentMessageType::PositionIntegrate, std::any(data)));
		}

		void onAttach(GameObject* owner) override
		{
			m_owner = owner;

			m_possibleMessages = { ComponentMessageType::VelocitySetLinear, ComponentMessageType::VelocitySetAngular };

			for (auto& msg : m_possibleMessages)
			{
				m_owner->getMap().insert(std::pair<ComponentMessageType, Component*>(msg, this));
				switch (msg)
				{
				case ComponentMessageType::VelocitySetLinear:
					m_msgsMap[msg] = [this](std::any data)
					{
						glm::vec3 linear = std::any_cast<glm::vec3>(data);
						m_linear = linear;
						return true;
					};
					break;

				case ComponentMessageType::VelocitySetAngular:
					m_msgsMap[msg] = [this](std::any data)
					{
						glm::vec3 angular = std::any_cast<glm::vec3>(data);
						m_angular = angular;
						return true;
					};
					break;
				}
			}
		}

		//! Gets linear velocity of a component \return linear velocity
		inline glm::vec3& getLinear() { return m_linear; }
		//! Gets angular velocity of a component \return angular velocity
		inline glm::vec3& getAngular() { return m_angular; }

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		//! Virtual destructor
		~VelocityComponent() {};
	};
}