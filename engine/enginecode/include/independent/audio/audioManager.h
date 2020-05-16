#pragma once
#include <map>

#include "audio/audioGeometryDefinition.h"
#include "audio/audioRollOff.h"
#include "glm/glm.hpp"

#include "fmod.hpp"
#include "fmod_studio.hpp"

namespace Engine {

	class AudioManager 
	{
	private:
		int m_nextChannelID = 0;
		std::string m_type;
		std::map<int, FMOD::Channel*> m_channels;
		std::map<std::string, FMOD::Sound*> m_sounds;
		std::map<std::string, FMOD::Geometry*> m_geometry;
		std::map<std::string, FMOD::Studio::EventInstance*> m_events;
		std::map<std::string, FMOD::Studio::Bank*> m_banks;

		int errorCheck(FMOD_RESULT result) const;
		FMOD_VECTOR GLMVecToFmod(const glm::vec3& vec);

	public:
		
		AudioManager(std::string layerName);

		void update();
		void updateLocation(float timestep, glm::vec3 position, glm::vec3 camForward, glm::vec3 camUp);
		void toggleChannelPause(int nChannelID);
		void togglePauseAllChannels();
		void setChannel3dPosition(int nChannelID, const glm::vec3& vPosition);
		bool isChannelPlaying(int nChannelID) const;
		bool isEventPlaying(const std::string& strEventName) const;
		void loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		void loadSound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false, float minDist = 0.25f,
			float maxDist = 10000.f, RollOff rollOff = RollOff::InverseTapered);
		void unloadSound(const std::string& strsoundName);
		int playSound(const std::string& strSoundName, const glm::vec3& vPos = glm::vec3{ 0 , 0 , 0 });
		void stopSound(const std::string& strSoundName);
		void loadEvent(const std::string& strEventName);
		void playEvent(const std::string& strEventName);
		void stopEvent(const std::string& strEventName, bool bImmediate = false);
		void getEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* value);
		void setEventParameter(const std::string& strSoundName, const std::string& strParameterName, float value);
		void setEventPosition(const std::string& strEventName, const glm::vec3& position);

		void addGeometry(const std::string label, const AudioGeometryDefinition& def);
		void moveGeometry(const std::string label, const glm::vec3& position);

		std::map<int, FMOD::Channel*>& getChannels() { return m_channels; }
	};
}
