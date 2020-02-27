#include "engine_pch.h"
#include "include\independent\audio\audioManager.h"

namespace Engine {

	int AudioManager::errorCheck(FMOD_RESULT result) const
	{
		return 0;
	}

	FMOD_VECTOR AudioManager::GLMVecToFmod(const glm::vec3& vec)
	{
		return FMOD_VECTOR();
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

	void AudioManager::loadSound(const std::string& strSoundName, bool b3d, bool bLooping , bool bStream , float minDist, float maxDist)
	{
		auto it = m_sounds.find(strSoundName);
		if (it != m_sounds.end())
			return;
		FMOD_MODE eMode = FMOD_DEFAULT;
		eMode |= b3d ? FMOD_3D : FMOD_2D;
		eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
		
		/*
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
		*/

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

	void AudioManager::set3dListenerAndOrientation(const glm::vec3 position, const glm::vec3& forward, const glm::vec3 up)
	{
		FMOD_VECTOR lastPos, lastVel, lastForward, lastUp;
		
		errorCheck(m_lowLevelSystem->get3DListenerAttributes(0, &lastPos, &lastVel, &lastForward, &lastUp));

		auto listenerPos = GLMVecToFmod(position);
		auto listenerForward = GLMVecToFmod(forward);
		auto listenerUp = GLMVecToFmod(up);

		FMOD_VECTOR vel;
		vel.x = (listenerPos.x - lastPos.x) * (1.0f / 60.f);
		vel.x = (listenerPos.y - lastPos.y) * (1.0f / 60.f);
		vel.x = (listenerPos.z - lastPos.z) * (1.0f / 60.f);

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


	void AudioManager::setEventParameter(const std::string& strEventName, const std::string& strParameterName, float value)
	{
		auto it = m_events.find(strEventName);
		if (it == m_events.end())
			return;
		errorCheck(it->second->setParameterByName(strParameterName.c_str(), value));
	}

}
