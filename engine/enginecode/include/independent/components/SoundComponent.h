#pragma once

/*! \file SoundComponent.h
\brief Defines a sound component
*/

#include "Component.h"
#include "audio/audioSound.h"
#include "audio/audioGeometryDefinition.h"
#include <audio/audioManager.h>
#include "PositionComponent.h"


namespace Engine
{
	/*! \class SoundComponent
	\brief Defines a sound component.

		Inherits from component.
	*/
	class SoundComponent : public Component
	{
	private:
		std::shared_ptr<AudioManager> m_audio; //!< Pointer to audio manager
		std::shared_ptr<Sound> m_sound; //!< Pointer to a sound
		glm::vec3 m_location; //!< Location

		std::string m_name; //!< name of the sound
		std::string m_layerName; //!< Name of the layer
		bool m_onAwake; //!< Should sound be played on awake
		float m_vol; //!< Volume
	public:
		/*! Custom constructor
		\param sound pointer to a sound
		\param name name of the sound
		\param audioType type of audio manager
		\param vol sound volume
		\param awake should be played on awake?
		*/
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

		//! Plays a sound \param vol volume to play a sound with
		void playSound(float vol) 
		{
			m_audio->playSound(m_sound->getName(), m_location);
			m_audio->getChannels().at(0)->setVolume(vol);
		};

		//! Stops a sound
		void stopSound() 
		{
			m_audio->stopSound(m_sound->getName());
		};

		void onUpdate(float timestep) override
		{
			m_location = m_owner->getComponent<PositionComponent>()->getCurrentPosition();
		};

		//! Gets audio source \return audio manager
		std::shared_ptr<AudioManager>& getSource()
		{
			return m_audio;
		}

		//! Gets audio type \return type of audio
		std::string getAudioType()
		{
			return m_layerName;
		}

		//! Gets sound name \return sound name
		std::string getSoundName()
		{
			return m_name;
		}

		//! Sets whether sound should play on awake \param awake new play on awake
		void setPlayOnAwake(bool awake)
		{
			m_onAwake = awake;
		}

		//! Gets whether sound plays on awake \return true if sound should play on awake
		bool playOnAwake()
		{
			return m_onAwake;
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		//! Sets sound volume \param newVol new sound volume
		void setVolume(float newVol)
		{
			m_vol = newVol;
		}

		//! Gets current volume \return sound volume
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