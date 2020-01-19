#pragma once

/*! \file PositionComponent.h
\brief Implements a component responsible for setting position of a GO.
*/

#include "Component.h"

namespace Engine
{
	/*! \class PositionComponent
	\brief Defines a component responsible for integrating and setting position of a GO. 
	
		Inherits from Component.
	*/
	class PositionComponent : public Component
	{
	private:
		glm::mat4 m_model; //!< Model matrix
		glm::mat4 m_translation; //!< Translation matrix
		glm::mat4 m_rotation; //!< Rotation matrix
		glm::mat4 m_scale; //!< Scale matrix

		glm::vec3 m_transformVec; //!< Position vector of a GO
		glm::vec3 m_rotationVec; //!< Rotation vector of GO
		glm::vec3 m_scaleVec; //!< Scale vector of GO

		//! Calculates model matrix to be displayed on a screen
		inline void calculateModel()
		{
			m_translation = glm::translate(glm::mat4(1.0f), m_transformVec);
			m_rotation = glm::toMat4(glm::quat(m_rotationVec));
			m_scale = glm::scale(m_model, m_scaleVec);

			m_model = m_scale * m_translation * m_rotation ;
		}

	public:
		/*! Custom constructor
		\param translation initial position
		\param rotation initial rotation
		\param scale initial scale
		*/
		PositionComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
			: m_transformVec(translation), m_rotationVec(rotation), m_scaleVec(scale), m_model(glm::mat4(1.0f))
		{
			m_rotationVec.x = glm::radians(m_rotationVec.x);
			m_rotationVec.y = glm::radians(m_rotationVec.y);
			m_rotationVec.z = glm::radians(m_rotationVec.z);
			calculateModel();
		}

		//! Gets current model matrix \return model matrix
		inline std::shared_ptr<glm::mat4> getTransform() { return std::make_shared<glm::mat4>(m_model); }

		void onUpdate(float timestep) override
		{
			std::pair<std::string, void*> data("u_model", (void*)&m_model[0][0]);
			ComponentMessage msg(ComponentMessageType::UniformSet, data);
			sendMessage(msg);
		}

		void onAttach(GameObject* owner) override
		{
			m_owner = owner;

			m_possibleMessages = { ComponentMessageType::PositionSet, ComponentMessageType::PositionIntegrate };

			for (auto& msg : m_possibleMessages)
			{
				m_owner->getMap().insert(std::pair<ComponentMessageType, Component*>(msg, this));
				switch (msg)
				{
				case ComponentMessageType::PositionSet:
					m_msgsMap[msg] = [this](std::any data)
					{
						glm::vec3 pos = std::any_cast<glm::vec3>(data);
						m_transformVec = pos;
						calculateModel();

						return true;
					};
					break;

				case ComponentMessageType::PositionIntegrate:
					m_msgsMap[msg] = [this](std::any data)
					{
						std::pair<glm::vec3, glm::vec3> vel = std::any_cast<std::pair<glm::vec3, glm::vec3>>(data);

						m_transformVec = vel.first;
						m_rotationVec.x = glm::radians(vel.second.x);
						m_rotationVec.y = glm::radians(vel.second.y);
						m_rotationVec.z = glm::radians(vel.second.z);

						calculateModel();
						return true;
					};
					break;
				}
			}

			std::pair<std::string, void*> data("u_model", (void*)&m_model[0][0]);
			ComponentMessage msg(ComponentMessageType::UniformSet, data);
			sendMessage(msg);
		}
	};
}