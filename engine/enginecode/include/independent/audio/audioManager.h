#pragma once

#include <map>
#include <string>
#include <glm/glm.hpp>
#include "systems/system.h"
#include "fmod.hpp"
#include "fmod_studio.hpp"

namespace Engine {
	
	class RollOff {
			
	};

	struct AudioGeometryDefinition {
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
		float directOcculsion = 1.0f;
		float reverbOcculsion = 0.3f;
	};

	class AudioManager : public System
	{
	private:
		FMOD::Studio::System* m_studioSystem;
		FMOD::System* m_lowLevelSystem;

		static const int m_maxChannels = 32;
		int m_nextChannelID = 0;

		std::map<std::string, FMOD::Sound*> m_sounds;
		std::map<int, FMOD::Channel*> m_channels;
		std::map<std::string, FMOD::Geometry*> m_geometry;
		std::map<std::string, FMOD::Studio::EventInstance*> m_events;
		std::map<std::string, FMOD::Studio::Bank*> m_banks;
		int errorCheck(FMOD_RESULT result) const;
		
		FMOD_VECTOR GLMVecToFmod(const glm::vec3& vec);

	public:
		
		void start(SystemSignal init = SystemSignal::None, ...);
		void stop(SystemSignal close = SystemSignal::None, ...);
		void update();
		void loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		void loadEvent(const std::string& strEventName);
		//void loadSound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false, float minDist = 0.25f,
		//	float maxDist = 10000.f, RollOff rollOff = RollOff::Inverse);
		void loadSound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false, float minDist = 0.25f,
			float maxDist = 10000.f);
		void unLoadSound(const std::string& strsoundName);
		void set3dListenerAndOrientation(const glm::vec3 position, const glm::vec3& forward, const glm::vec3 up);
		void addGeometry(const std::string label, const AudioGeometryDefinition& def);
		void moveGeometry(const std::string label, const glm::vec3& position);
		int playSound(const std::string& strSoundName, const glm::vec3& vPos = glm::vec3{ 0 , 0 , 0 });
		void playEvent(const std::string& strEventName);
		void toggleChannelPause(int nChannelID);
		void stopEvent(const std::string& strEventName, bool bImmediate = false);
		void getEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* value);
		void setEventParameter(const std::string& strSoundName, const std::string& strParameterName, float value);
		void setEventPosition(const std::string& strEventName, const glm::vec3& position);
		void togglePauseAllChannels();
		void setChannel3dPosition(int nChannelID, const glm::vec3& vPosition);
		bool isPlaying(int nChannelID) const;
		
		bool isEventPlaying(const std::string& strEventName) const;
	};
}

