#include "engine_pch.h"
#include "include\independent\audio\audioManager.h"
#include "data/json/JsonLayer.h"

namespace Engine {

	int AudioManager::errorCheck(FMOD_RESULT result) const
	{
		if (result != FMOD_OK) {
			LogError("FMOD Error");
			return 1;
		}
		return 0;
	}

	void AudioManager::start(SystemSignal init, ...)
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

	void AudioManager::stop(SystemSignal close, ...)
	{

	}

	void AudioManager::update()
	{
		std::vector<std::map<int, FMOD::Channel*>::iterator> l_stoppedChannels;
		for (auto it = m_channels.begin(); it != m_channels.end(); ++it)
		{
			bool isPlaying = false;
			it->second->isPlaying(&isPlaying);
			if (!isPlaying)
			{
				l_stoppedChannels.push_back(it);
			}
		}
		for (auto& it : l_stoppedChannels)
		{
			m_channels.erase(it);
		}

		errorCheck(m_lowLevelSystem->update());

		errorCheck(m_studioSystem->update());
	}

	void AudioManager::loadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream, float minDist, float maxDist, RollOff rollOff)
	{
		auto it = m_sounds.find(strSoundName);
		if (it != m_sounds.end())
			return;
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
		errorCheck(m_lowLevelSystem->createSound(strSoundName.c_str(), eMode, 0, &sound));

		if (b3d)
		{
			sound->set3DMinMaxDistance(minDist, maxDist);
		}
		if (sound != nullptr) {
			m_sounds[strSoundName] = sound;
		}
	}

	void AudioManager::unLoadSound(const std::string& strSoundName)
	{
		auto it = m_sounds.find(strSoundName);
		if (it == m_sounds.end())
			return;
		errorCheck(it->second->release());
		m_sounds.erase(it);

	}

	void AudioManager::set3dListenerAndOrientation(const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up)
	{
		FMOD_VECTOR lastPos, lastVel, lastForward, lastUp;
		
		errorCheck(m_lowLevelSystem->get3DListenerAttributes(0, &lastPos, &lastVel, &lastForward, &lastUp));
		
		/*
		auto listenerPos = GLMVecToFmod(l.getCamera()->getCamera()->getPosition());
		auto listenerForward = GLMVecToFmod(glm::vec3(l.getCamera()->getCamera()->getPosition().x, l.getCamera()->getCamera()->getPosition().y, 1.0f));
		auto listenerUp = GLMVecToFmod(glm::vec3(l.getCamera()->getCamera()->getPosition().x, 1.0f, l.getCamera()->getCamera()->getPosition().z));
		*/

		auto listenerPos = GLMVecToFmod(position);
		auto listenerForward = GLMVecToFmod(forward);
		auto listenerUp = GLMVecToFmod(up);


		FMOD_VECTOR vel;
		vel.x = (listenerPos.x - lastPos.x) * (1.0f / 60.f);
		vel.y = (listenerPos.y - lastPos.y) * (1.0f / 60.f);
		vel.z = (listenerPos.z - lastPos.z) * (1.0f / 60.f);

		errorCheck(m_lowLevelSystem->set3DListenerAttributes(0, &listenerPos, &vel, &listenerForward, &listenerUp));

	}

	void AudioManager::addGeometry(const std::string label, const AudioGeometryDefinition& def)
	{
		FMOD::Geometry* geometry;
		
		int numPolys = def.indices.size() / 3;

		errorCheck(m_lowLevelSystem->createGeometry(numPolys, def.vertices.size(), &geometry));

		m_geometry[label] = geometry;
		FMOD_VECTOR triangle[3];
		FMOD_VECTOR vert = { 0,0,0 };
		int polygonIndex;

		for (int i = 0, j = 0; i < def.vertices.size(); i++)
		{
			vert.x = def.vertices[i]; i++;
			vert.y = def.vertices[i]; i++;
			vert.z = def.vertices[i];

			triangle[j] = vert;
			j++;

			if (j == 3)
			{
				geometry->addPolygon(def.directOcculsion, def.reverbOcculsion, true, 3, triangle, &polygonIndex);
				j = 0;
			}
		}
		geometry->setScale(&GLMVecToFmod(def.scale));
		geometry->setPosition(&GLMVecToFmod(def.position));
	}

	int AudioManager::playSound(const std::string& strSoundName, const glm::vec3& vPos)
	{
		int channelId = m_nextChannelID++;
		auto it = m_sounds.find(strSoundName);
		if (it == m_sounds.end())
		{
			loadSound(strSoundName);
			it = m_sounds.find(strSoundName);
			if (it == m_sounds.end())
			{
				return channelId;
			}
		}
		FMOD::Channel* pChannel = nullptr;
		errorCheck(m_lowLevelSystem->playSound(it->second, nullptr, true, &pChannel));
		if (pChannel)
		{
			FMOD_MODE currMode;
			it->second->getMode(&currMode);
			if (currMode & FMOD_3D) {
				FMOD_VECTOR position = GLMVecToFmod(vPos);
				errorCheck(pChannel->set3DAttributes(&position, nullptr));
			}
			errorCheck(pChannel->setPaused(false));
			m_channels[channelId] = pChannel;
		}
		return channelId;
	}

	void AudioManager::playEvent(const std::string& strEventName)
	{
		auto it = m_events.find(strEventName);
		if (it == m_events.end()) {
			loadEvent(strEventName);
			it = m_events.find(strEventName);
			if (it == m_events.end())
				return;
		}
		it->second->start();
	}

	void AudioManager::stopEvent(const std::string& strEventName, bool bImmediate)
	{
		auto it = m_events.find(strEventName);
		if (it == m_events.end())
			return;
		FMOD_STUDIO_STOP_MODE eMode;
		eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
		errorCheck(it->second->stop(eMode));
	}


	void AudioManager::loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
	{
		auto it = m_banks.find(strBankName);
		if (it != m_banks.end())
			return;
		FMOD::Studio::Bank* bank;
		errorCheck(m_studioSystem->loadBankFile(strBankName.c_str(), flags, &bank));
		if (bank) {
			m_banks[strBankName] = bank;
		}
	}

	void AudioManager::loadEvent(const std::string& strEventName)
	{
		auto it = m_events.find(strEventName);
		if (it != m_events.end())
			return;
		FMOD::Studio::EventDescription* eventDescription = NULL;
		errorCheck(m_studioSystem->getEvent(strEventName.c_str(), &eventDescription));
		if (eventDescription) {
			FMOD::Studio::EventInstance* eventInstance = NULL;
			errorCheck(eventDescription->createInstance(&eventInstance));
			if (eventInstance) {
				m_events[strEventName] = eventInstance;
			}
		}
	}

	bool AudioManager::isEventPlaying(const std::string& strEventName) const
	{
		auto it = m_events.find(strEventName);
		if (it == m_events.end())
			return false;

		FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
		if (it->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
			return true;
		}
		return false;
	}

	FMOD_VECTOR AudioManager::GLMVecToFmod(const glm::vec3& vec)
	{
		FMOD_VECTOR fmod;
		fmod.x = vec.x;
		fmod.y = vec.y;
		fmod.z = vec.z;
		return fmod;
	}
	
	void AudioManager::setChannel3dPosition(int channelID, const glm::vec3& vPos)
	{
		auto it = m_channels.find(channelID);
		if (it == m_channels.end())
			return;

		FMOD_VECTOR position = GLMVecToFmod(vPos);
		errorCheck(it->second->set3DAttributes(&position, NULL));
	}

	void AudioManager::setEventParameter(const std::string& strEventName, const std::string& strParameterName, float value)
	{
		auto it = m_events.find(strEventName);
		if (it == m_events.end())
			return;
		errorCheck(it->second->setParameterByName(strParameterName.c_str(), value));
	}

	void AudioManager::getEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* value)
	{
	}

	void AudioManager::setEventPosition(const std::string& strEventName, const glm::vec3& position)
	{
	}



	void AudioManager::togglePauseAllChannels()
	{
	}

	void AudioManager::toggleChannelPause(int nChannelID)
	{

	}

	bool AudioManager::isPlaying(int nChannelID) const
	{
		return false;
	}

	void AudioManager::moveGeometry(const std::string label, const glm::vec3& position)
	{

	}



}
