#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "assimp/scene.h"
#include <vector>

namespace Engine
{
	class AnimatorComponent : public Component
	{
	private:
		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;
		
		glm::mat4 m_finalTransform;

		std::vector<aiVectorKey> m_posKeys;
		std::vector<aiQuatKey> m_rotKeys;
		std::vector<aiVectorKey> m_scaleKeys;

		void calculateTransform()
		{
			glm::mat4 translationM = glm::translate(glm::mat4(1.0f), m_position);
			glm::mat4 rotationM = glm::toMat4(m_rotation);
			glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), m_scale);

			m_finalTransform = translationM * rotationM * scaleM;
		}
	public:
		AnimatorComponent(std::vector<aiVectorKey> posKeys, std::vector<aiQuatKey> rotKeys, std::vector<aiVectorKey> scaleKeys)
			: m_posKeys(posKeys), m_rotKeys(rotKeys), m_scaleKeys(scaleKeys), m_finalTransform(glm::mat4(1.0f))
		{};
	
		void onAttach(GameObject* owner) override 
		{ 
			m_owner = owner; 
			m_possibleMessages = { ComponentMessageType::BoneSet };

			for (auto& msg : m_possibleMessages)
			{
				m_owner->getMap().insert(std::pair<ComponentMessageType, Component*>(msg, this));
				switch (msg)
				{
				case ComponentMessageType::BoneSet:
					m_msgsMap[msg] = [this](std::any data)
					{
						calculateTransform();
						return true;
					};
					break;
				}
			}
		}
		void onDetach() override {};
		void onUpdate(float timestep) override 
		{
			// interpolate
			// send message
		};

		void onEvent(Event& e) override {};
	};
}