#pragma once

#include <string>
#include <glm/glm.hpp>
#include "fmod.hpp"
#include "fmod.hpp"
#include "audio/audioRollOff.h"

namespace Engine {

	class Sound
	{
	private:
		FMOD::Sound* m_sound;

		std::string m_name;
		bool m_b3d;
		bool m_Looping;
		bool m_Stream;
		float m_minDist;
		float m_maxDist;
		RollOff m_rollOff;

		int errorCheck(FMOD_RESULT result) const;

	public:

		Sound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false, float minDist = 0.25f,
			float maxDist = 10000.f, RollOff rollOff = RollOff::InverseTapered);

		static Sound* create(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false, float minDist = 0.25f,
			float maxDist = 10000.f, RollOff rollOff = RollOff::InverseTapered);
		
		std::string getName() { return m_name; }
		bool get3D() { return m_b3d; } 
		bool getLooping() { return m_Looping; } 
		bool getStream() { return m_Stream; } 
		float getMinDist() { return m_minDist; } 
		float getMaxDist() { return m_maxDist; } 
		RollOff getRollOff() { return m_rollOff; } 
		FMOD::Sound* getFmodSound() { return m_sound; }
	};

}

