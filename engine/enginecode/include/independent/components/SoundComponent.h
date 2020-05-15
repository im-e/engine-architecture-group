#pragma once

/*! \file SoundComponent.h
\brief 

*/
#include "Component.h"
#include "audio/audioSound.h"
#include "audio/audioGeometryDefinition.h"
#include <audio/audioManager.h>
#include "PositionComponent.h"


namespace Engine
{
	/*! \class SoundComponent
		\brief
	*/
	class SoundComponent : public Component
	{
	private:
		std::shared_ptr<AudioManager> m_audio;
		std::shared_ptr<Sound> m_sound;
		int m_channel;

		std::string m_name;
		glm::vec3 m_location;

		
	public:
		
		SoundComponent(const std::shared_ptr<Sound>& sound, std::string name, std::string audioType, glm::vec3 location)
			: m_sound(sound), m_name(name), m_location(location)
		{
			m_audio.reset(new AudioManager(audioType));
		}

		void onAttach(GameObject* owner) override
		{
			m_owner = owner;
		}

		void playSound() 
		{
			m_audio->playSound(m_sound->getName(), m_location);
		};

		void stopSound() 
		{
			m_audio->stopSound(m_sound->getName());
		};

		void onUpdate(float timestep) override
		{
			m_location = m_owner->getComponent<PositionComponent>()->getCurrentPosition();
		};

		std::shared_ptr<AudioManager>& getSource()
		{
			return m_audio;
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

	};
}

// Sound component class
// should be able to be added to gameobject
// should hold any properties allowing it to be saved

/*
	Properties:
	sound name (which you already have loaded from JsonLoader)
	OPTIONAL audio geometry def

	Functionality:
	Play sound
	Stop sound
	Update location (should be updated automatically)

	Sound location = gameobject's loacation
*/