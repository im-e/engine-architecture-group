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

			m_possibleMessages = { ComponentMessageType::VelocitySetLinear, ComponentMessageType::VelocitySetAngular,
									ComponentMessageType::AIPositionSet,	ComponentMessageType::AIRotationSet };

			glm::vec3 temp = m_linear;

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

				case ComponentMessageType::AIPositionSet:
					m_msgsMap[msg] = [temp, owner, this](std::any data)
					{
						m_linear = temp;
						glm::vec3 desired = std::any_cast<glm::vec3>(data);
						glm::vec3 current = owner->getComponent<PositionComponent>()->getCurrentPosition();
						
						glm::vec3 diff = current - desired;

						if (diff.x > 0.001f || diff.x < -0.001f)
						{
							if (diff.x > 0.0f)
							{
								if (m_linear.x > 0.0f)
								{
									m_linear.x *= -1;
								}
								else
								{
									m_linear.x = m_linear.x;
								}						
							}
							else
							{
								if (m_linear.x < 0.0f)
								{
									m_linear.x *= -1; // reverse
								}
								else
								{
									m_linear.x = m_linear.x;
								}
							}
						}
						else
						{
							m_linear.x = 0.0f;
						}

						if (diff.y > 0.001f || diff.y < -0.001f)
						{
							if (diff.y > 0.0f)
							{
								if (m_linear.y > 0.0f)
								{
									m_linear.y *= -1;
								}
								else
								{
									m_linear.y = m_linear.y;
								}
							}
							else
							{
								if (m_linear.y < 0.0f)
								{
									m_linear.y *= -1; // reverse
								}
								else
								{
									m_linear.y = m_linear.y;
								}
							}
						}
						else
						{
							m_linear.y = 0.0f;
						}

						if (diff.z > 0.001f || diff.z < -0.001f)
						{
							if (diff.z > 0.0f)
							{
								if (m_linear.z > 0.0f)
								{			 
									m_linear.z *= -1;
								}
								else
								{
									m_linear.z = m_linear.z;
								}
							}
							else
							{
								if (m_linear.z < 0.0f)
								{
									m_linear.z *= -1; // reverse
								}
								else
								{
									m_linear.z = m_linear.z;
								}
							}
						}
						else
						{
							m_linear.z = 0.0f;
						}

						m_linear = glm::normalize(m_linear);

						return true;
					};
					break;

				case ComponentMessageType::AIRotationSet:
					m_msgsMap[msg] = [this](std::any data)
					{
						// TODO rotation
							// How to???
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

		//! Sets linear velocity \param newLin new linear velocity
		inline void setLinear(glm::vec3 newLin)
		{
			m_linear = newLin;
		}

		//! Sets angular velocity \param newAng new angular velocity
		inline void setAngular(glm::vec3 newAng)
		{
			m_angular = newAng;
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		//! Virtual destructor
		~VelocityComponent() {};
	};
}