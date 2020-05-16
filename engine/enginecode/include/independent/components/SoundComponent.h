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
		glm::vec3 m_location;

		std::string m_name;
		std::string m_layerName;
		bool m_onAwake;
		float m_vol;
	public:
		
		SoundComponent(const std::shared_ptr<Sound>& sound, std::string name, std::string audioType, float vol, bool awake)
			: m_sound(sound), m_name(name), m_layerName(audioType), m_onAwake(awake), m_vol(vol)
		{
			m_audio.reset(new AudioManager(audioType));			
		}

		void onAttach(GameObject* owner) override
		{
			m_owner = owner;
			m_location = m_owner->getComponent<PositionComponent>()->getCurrentPosition();

			if (m_onAwake)
			{
				m_audio->playSound(m_sound->getName(), m_location);
				m_audio->getChannels().at(0)->setVolume(m_vol);
			}
		}

		void playSound(float vol) 
		{
			m_audio->playSound(m_sound->getName(), m_location);
			m_audio->getChannels().at(0)->setVolume(vol);
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

		std::string getAudioType()
		{
			return m_layerName;
		}

		std::string getSoundName()
		{
			return m_name;
		}

		void setPlayOnAwake(bool awake)
		{
			m_onAwake = awake;
		}

		bool playOnAwake()
		{
			return m_onAwake;
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		void setVolume(float newVol)
		{
			m_vol = newVol;
		}

		float getVolume()
		{
			return m_vol;
		}

		void onDetach() override
		{
			m_audio->stopSound(m_sound->getName());

			auto iter = m_owner->getMap().begin();
			while (iter != m_owner->getMap().end())
			{
				if ((*iter).second == this)
				{
					iter = m_owner->getMap().erase(iter);
					--iter;
				}
				else
				{
					++iter;
				}
			}
		}

	};
}