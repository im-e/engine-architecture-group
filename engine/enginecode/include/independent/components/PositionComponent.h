#pragma once

/*! \file PositionComponent.h
\brief Implements a component responsible for setting position of a GO.
*/

#include "Component.h"
//#include "RigidbodyComponent.h"

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

		glm::vec3 m_initialTransformVec; //!< Initial position vector of a GO
		glm::vec3 m_initialRotationVec; //!< Initial rotation vector of GO
		glm::vec3 m_initialScaleVec; //!< Initial scale vector of GO

		//! Calculates model matrix to be displayed on a screen
		inline void calculateModel()
		{
			m_translation = glm::translate(glm::mat4(1.0f), m_transformVec);
			m_rotation = glm::toMat4(glm::quat(m_rotationVec));
			m_scale = glm::scale(m_scaleVec);

			m_model = m_translation * m_rotation * m_scale;
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

			m_initialTransformVec = m_transformVec;
			m_initialRotationVec = m_rotationVec;
			m_initialScaleVec = m_scaleVec;

			m_rotationVec.x = glm::radians(m_rotationVec.x);
			m_rotationVec.y = glm::radians(m_rotationVec.y);
			m_rotationVec.z = glm::radians(m_rotationVec.z);
			calculateModel();
		}

		//! Gets current model matrix \return model matrix
		inline std::shared_ptr<glm::mat4> getTransform() { return std::make_shared<glm::mat4>(m_model); }

		void onUpdate(float timestep) override
		{
			auto mat = m_owner->getComponent<MaterialComponent>();
			if (mat != nullptr)
			{
				std::pair<std::string, void*> data("u_model", (void*)&m_model[0][0]);
				ComponentMessage msg(ComponentMessageType::UniformSet, data);
				sendMessage(msg);
			}
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

				case ComponentMessageType::PositionIntegrate: //OpenGL
					m_msgsMap[msg] = [this](std::any data)
					{
						std::pair<glm::vec3, glm::vec3> vel = std::any_cast<std::pair<glm::vec3, glm::vec3>>(data);

						m_transformVec += vel.first;
						m_rotationVec.x += glm::radians(vel.second.x);
						m_rotationVec.y += glm::radians(vel.second.y);
						m_rotationVec.z += glm::radians(vel.second.z);

						calculateModel();
						return true;
					};
				}
			}

			std::pair<std::string, void*> data("u_model", (void*)&m_model[0][0]);
			ComponentMessage msg(ComponentMessageType::UniformSet, data);
			sendMessage(msg);
		}

		void onDetach() override
		{
			for (auto iter = m_owner->getMap().begin(); iter != m_owner->getMap().end(); ++iter)
			{
				if ((*iter).second == this)
				{
					iter = m_owner->getMap().erase(iter);
					--iter;
				}
			}
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		//! Returns position \return current position
		inline glm::vec3& getCurrentPosition()
		{
			return m_transformVec;
		}

		//! Returns rotation \return current rotation
		inline glm::vec3 getCurrentRotation()
		{
			return glm::degrees(m_rotationVec);
		}

		//! Returns scale \return current scale
		inline glm::vec3 getCurrentScale()
		{
			return m_scaleVec;
		}

		//! Sets position \param newPos new position
		inline void setPosition(glm::vec3 newPos)
		{
			m_initialTransformVec = newPos;
			m_transformVec = newPos;
			calculateModel();
		}

		//! Sets rotation \param newRot new rotation
		inline void setRotation(glm::vec3 newRot)
		{
			m_initialRotationVec = newRot;

			m_rotationVec.x = glm::radians(newRot.x);
			m_rotationVec.y = glm::radians(newRot.y);
			m_rotationVec.z = glm::radians(newRot.z);
			calculateModel();
		}

		//! Sets scale \param newScale new scale
		inline void setScale(glm::vec3 newScale)
		{
			m_initialScaleVec = newScale;
			m_scaleVec = newScale;
			calculateModel();
		}

		//! Gets initial position \return initial position
		inline glm::vec3 getInitialPosition()
		{
			return m_initialTransformVec;
		}

		//! Gets initial rotation \return initial rotation
		inline glm::vec3 getInitialRotation()
		{
			return m_initialRotationVec;
		}

		//! Gets initial scale \return initial scale
		inline glm::vec3 getInitialScale()
		{
			return m_initialScaleVec;
		}
	};
}