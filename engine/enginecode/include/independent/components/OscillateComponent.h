#pragma once

/*! \file OscillateComponent.h
\brief Implements a component responsible for moving a GO up and down.
*/

#include "Component.h"

namespace Engine
{
	/*! \class OscillateComponent
	\brief Defines component to move a GO up and down.
	
		Inherits from Component
	*/
	class OscillateComponent : public Component
	{
	public:
		//! Possible states of a component
		enum State { STOPPED = 0, DOWN = 1, UP = 2 };

	private:
		float m_timeAccumulated = 0.0f; //!< Accumulated time
		float m_maxTime; //!< Maximum time to pass before a GO changes direction
		bool m_setTexture; //!< Should a component change textures when oscillating?
		State m_state; //!< Current state

		glm::vec3 m_linearSpeed; //!< Linear speed of a GO
	public:
		/*! Custom constructor
		\param initialState state of a GO at the beginning
		\param currentTime marks current time that has passed (useful for synchronizing movement)
		\param setTexture indicates whether component shall send messages to change texture
		\param linearSpeed initial linear speed of a component.
		*/
		OscillateComponent(OscillateComponent::State initialState, float currentTime, float maxTime, bool setTexture, glm::vec3 linearSpeed) 
			: m_state(initialState), m_timeAccumulated(currentTime), m_maxTime(maxTime), m_setTexture(setTexture), m_linearSpeed(linearSpeed)
		{
		};
		
		void onAttach(GameObject* owner) override
		{
			m_owner = owner;
			ChangeDirection();
		}

		void onUpdate(float timestep) override 
		{
			m_timeAccumulated += timestep;

			if (m_timeAccumulated >= m_maxTime)
			{
				ChangeState();
				ChangeDirection();
				m_timeAccumulated = 0.0f;

			}
		};

		void onEvent(Event& e) override {};

		//! Changes state according after timer exceeds maximum time
		void ChangeState()
		{
			switch (m_state)
			{
			case DOWN:
				m_state = UP;
				break;

			case UP:
				m_state = DOWN;
				break;

			case STOPPED:

				break;
			}
		}

		//! Changes direction after timer exceeds maximum time
		void ChangeDirection()
		{
			switch (m_state)
			{
			case DOWN:
				sendMessage(ComponentMessage(ComponentMessageType::VelocitySetLinear,
					std::any(glm::vec3(m_linearSpeed.x, -m_linearSpeed.y, m_linearSpeed.z))));			
				break;

			case UP:
				sendMessage(ComponentMessage(ComponentMessageType::VelocitySetLinear,
					std::any(glm::vec3(m_linearSpeed.x, m_linearSpeed.y, m_linearSpeed.z))));
				break;

			case STOPPED:
				sendMessage(ComponentMessage(ComponentMessageType::VelocitySetLinear,
					std::any(glm::vec3(0.0f))));
				break;
			}

			if(m_setTexture == true)
				sendMessage(ComponentMessage(ComponentMessageType::TextureSet, (unsigned int)m_state));
		
		}

	};
}