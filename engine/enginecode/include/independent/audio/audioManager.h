#pragma once

/*! \file audioManager.h
\brief Defines an audio manager
*/

#include <map>
#include "audio/audioGeometryDefinition.h"
#include "audio/audioRollOff.h"
#include "glm/glm.hpp"

#include "fmod.hpp"
#include "fmod_studio.hpp"

namespace Engine {

	/*! \class AudioManager
	\brief Provides definition for audio manager
	*/
	class AudioManager 
	{
	private:
		//! Next channel
		int m_nextChannelID = 0;
		std::string m_type; //!< Audio manager type
		std::map<int, FMOD::Channel*> m_channels; //!< Map of channels
		std::map<std::string, FMOD::Sound*> m_sounds; //!< Map of sounds
		std::map<std::string, FMOD::Geometry*> m_geometry; //!< Map of geometries
		std::map<std::string, FMOD::Studio::EventInstance*> m_events; //!< Map of events
		std::map<std::string, FMOD::Studio::Bank*> m_banks; //!< Map of banks

		//! Function to check for errors. \param result FMOD result \return true if error occurred
		int errorCheck(FMOD_RESULT result) const;
		//! Converst glm vector to FMOD vectors \param vec glm vector \return converted vector
		FMOD_VECTOR GLMVecToFmod(const glm::vec3& vec);

	public:
		//! Custom constructor \param layerName name of the layer audio manager exists withing
		AudioManager(std::string layerName);

		//! Updates manager per frame
		void update();
		/*! Updates location of listener with relation to manager
		\param timestep delta time
		\param position listener position
		\param camForward listener forward vector
		\param camUp listener up vector
		*/
		void updateLocation(float timestep, glm::vec3 position, glm::vec3 camForward, glm::vec3 camUp);
		//! Pause playing sound on a given channel \param nChannelID channel to be pause
		void toggleChannelPause(int nChannelID);
		//! Pause all channels
		void togglePauseAllChannels();
		//! Sets position of a channel in 3D space \param nChannelID channel to be set \param vPosition position in 3D space
		void setChannel3dPosition(int nChannelID, const glm::vec3& vPosition);
		//! Is channel playing? \param nChannelID channel to be checked
		bool isChannelPlaying(int nChannelID) const;
		//! Is event playing? \param strEventName name of the event
		bool isEventPlaying(const std::string& strEventName) const;
		//! Loads a bank \param strBankName name of the bank \param flags FMOD flags
		void loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		/*! Loads a sound
		\param strSoundName soundName
		\param b3d is it 3D sound?
		\param bLooping should the sound loop?
		\param bStream is sound streamed?
		\param minDist minimum distance to hear sound from
		\param maxDist maximum distance to hear sound from
		\param rollOff sound roll off type
		*/
		void loadSound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false, float minDist = 0.25f,
			float maxDist = 10000.f, RollOff rollOff = RollOff::InverseTapered);
		//! Unloads sound \param strSoundName name of the sound to unload
		void unloadSound(const std::string& strsoundName);
		//! Plays a sound \param strSoundName name of the sound to be player \param vPos position of the sound to be played at \return channelID of played sound
		int playSound(const std::string& strSoundName, const glm::vec3& vPos = glm::vec3{ 0 , 0 , 0 });
		//! Stops a sound \param strSoundName name of the sound to be stopped
		void stopSound(const std::string& strSoundName);
		//! Loads an event \param strEventName name of the event to be loaded
		void loadEvent(const std::string& strEventName);
		//! Plays an event \param strEventName name of the event to be player
		void playEvent(const std::string& strEventName);
		//! Stops an event \param strEventName name of the event to be stopped \param bImmediate should an event stop immediately after playing?
		void stopEvent(const std::string& strEventName, bool bImmediate = false);
		//! Sets an event paramteter \param strEventName name of the event \param strParameterName name of the event parameter \param value parameter
		void setEventParameter(const std::string& strEventName, const std::string& strParameterName, float value);

		//! Adds a geometry for sound obstruction \param label name of the geometry \param def Definition of a geometry
		void addGeometry(const std::string label, const AudioGeometryDefinition& def);

		//! Get all channels in audio manager \return channels collection
		std::map<int, FMOD::Channel*>& getChannels() { return m_channels; }
	};
}
