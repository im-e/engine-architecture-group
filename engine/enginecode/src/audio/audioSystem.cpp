#include "engine_pch.h"
#include "audio/audioSystem.h"

namespace Engine {

	int AudioSystem::errorCheck(FMOD_RESULT result) const
	{
		if (result != FMOD_OK) {
			LogError("FMOD Error");
			return 1;
		}
		return 0;
	}

	void AudioSystem::start(SystemSignal init, ...)
	{
		errorCheck(FMOD::Studio::System::create(&m_studioSystem));
		errorCheck(m_studioSystem->initialize(m_maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL));

		unsigned int version;
		errorCheck(FMOD::System_Create(&m_lowLevelSystem));
		errorCheck(m_lowLevelSystem->getVersion(&version));

		if (version < FMOD_VERSION)
		{
			LogError("FMOD lib version {0} doesn't match header version {1}", version, FMOD_VERSION);
		}

		errorCheck(m_lowLevelSystem->init(m_maxChannels, FMOD_INIT_NORMAL, NULL));

		errorCheck(m_lowLevelSystem->set3DSettings(1.f, 1.f, 1.f));

		errorCheck(m_lowLevelSystem->setGeometrySettings(50.f));
	}

	void AudioSystem::stop(SystemSignal close, ...)
	{

	}

	void AudioSystem::update()
	{
		errorCheck(m_lowLevelSystem->update());

		errorCheck(m_studioSystem->update());
	}

	



}
