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
		m_name = strSoundName;
		m_b3d = b3d;
		m_Looping = bLooping;
		m_Stream = bStream;
		m_minDist = minDist;
		m_maxDist = maxDist;
		m_rollOff = rollOff;


		FMOD_MODE eMode = FMOD_DEFAULT;
		eMode |= m_b3d ? FMOD_3D : FMOD_2D;
		eMode |= m_Looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		eMode |= m_Stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;


		switch (m_rollOff)
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
		if(!m_b3d) errorCheck(Engine::Application::getInstance().getAudioSystem()->get2DSystem().createSound(m_name.c_str(), eMode, 0, &sound));
		if (m_b3d)
		{
			errorCheck(Engine::Application::getInstance().getAudioSystem()->getLowLevelSystem().createSound(m_name.c_str(), eMode, 0, &sound));
			sound->set3DMinMaxDistance(m_minDist, m_maxDist);
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