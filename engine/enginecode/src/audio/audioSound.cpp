#include "engine_pch.h"
#include "audio/audioSound.h"
#include "core/application.h"

namespace Engine {

	int Sound::errorCheck(FMOD_RESULT result) const
	{
		if (result != FMOD_OK) {
			LogError("FMOD Error");
			return 1;
		}
		return 0;
	}

	Sound::Sound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream, float minDist, float maxDist, RollOff rollOff)
	{
		FMOD_MODE eMode = FMOD_DEFAULT;
		eMode |= b3d ? FMOD_3D : FMOD_2D;
		eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;


		switch (rollOff)
		{
		case RollOff::Linear:
			eMode |= FMOD_3D_LINEARROLLOFF;
			break;
		case RollOff::LinearSquared:
			eMode |= FMOD_3D_LINEARSQUAREROLLOFF;
			break;
		case RollOff::InverseTapered:
			eMode |= FMOD_3D_INVERSETAPEREDROLLOFF;
			break;
		}


		FMOD::Sound* sound = nullptr;
		errorCheck(Engine::Application::getInstance().getAudioSystem()->getLowLevelSystem().createSound(strSoundName.c_str(), eMode, 0, &sound));

		if (b3d)
		{
			sound->set3DMinMaxDistance(minDist, maxDist);
		}
		if (sound != nullptr) {

			m_sound =  sound;
		}

	}

	Sound* Sound::create(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream, float minDist, float maxDist, RollOff rollOff)
	{
		return new Sound(strSoundName, b3d, bLooping, bStream, minDist, maxDist, rollOff);
	}


}