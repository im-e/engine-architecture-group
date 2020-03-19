#pragma once

#include <string>
#include <glm/glm.hpp>
#include "fmod.hpp"
#include "fmod.hpp"
#include "audio/audioRollOff.h"
#include "core/application.h"

namespace Engine {

	class Sound
	{
	protected:
		FMOD::Sound* m_sound;

	public:
		static Sound* create(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false, float minDist = 0.25f,
			float maxDist = 10000.f, RollOff rollOff = RollOff::InverseTapered);
		void unload();
		int play();

	};

}

