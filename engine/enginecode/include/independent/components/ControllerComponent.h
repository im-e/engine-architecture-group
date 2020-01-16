#pragma once

/*! \file ControllerComponent.h
\brief Allows to manually control GameObjects

*/
#include "Component.h"

#include "platform/InputPoller.h"
#include "platform/GLFW_KeyCodes.h"

namespace Engine
{
	/*! \class ControllerComponent
		\brief Provides implementation to manually control GameObject
	*/
	class ControllerComponent : public Component
	{
	private:
		float m_moveSpeed; //!< Move speed of a GO
		float m_rotationSpeed; //!< Rotation speed of a GO

	public:
		//! Constructor \param moveSpeed movement speed \param rotSpeed rotation speed
		ControllerComponent(float moveSpeed, float rotSpeed) : m_moveSpeed(moveSpeed), m_rotationSpeed(rotSpeed) {}

		void onUpdate(float timestep) override
		{
			float xPos = 0.0f;
			float yPos = 0.0f;
			float zPos = 0.0f;

			float xRot = 0.0f;
			float yRot = 0.0f;
			float zRot = 0.0f;

			if (InputPoller::isKeyPressed(NG_KEY_I))
			{
				zPos += m_moveSpeed * timestep;
			}
			if (InputPoller::isKeyPressed(NG_KEY_K))
			{
				zPos -= m_moveSpeed * timestep;
			}
			if (InputPoller::isKeyPressed(NG_KEY_J))
			{
				xPos += m_moveSpeed * timestep;
			}
			if (InputPoller::isKeyPressed(NG_KEY_L))
			{
				xPos -= m_moveSpeed * timestep;
			}
			if (InputPoller::isKeyPressed(NG_KEY_SPACE))
			{
				yPos += m_moveSpeed * timestep;
			}
			if (InputPoller::isKeyPressed(NG_KEY_LEFT_CONTROL))
			{
				yPos -= m_moveSpeed * timestep;
			}

			if (InputPoller::isKeyPressed(NG_KEY_Z))
			{
				xRot += m_rotationSpeed * timestep;
			}
			if (InputPoller::isKeyPressed(NG_KEY_X))
			{
				yRot += m_rotationSpeed * timestep;
			}
			if (InputPoller::isKeyPressed(NG_KEY_C))
			{
				zRot += m_rotationSpeed * timestep;
			}

			sendMessage(ComponentMessage(ComponentMessageType::VelocitySetLinear, glm::vec3(xPos, yPos, zPos)));
			sendMessage(ComponentMessage(ComponentMessageType::VelocitySetAngular, glm::vec3(xRot, yRot, zRot)));
		}

		void onAttach(GameObject* owner) override
		{
			m_owner = owner;
		}
	};
}
