#pragma once

#include <map>
#include <string>
#include <glm/glm.hpp>
#include "systems/system.h"
#include "audio/audioRollOff.h"
#include "audio/audioGeometryDefinition.h"
#include "fmod.hpp"
#include "fmod_studio.hpp"

namespace Engine {
	
	class AudioSystem : public System
	{
	private:
		FMOD::Studio::System* m_studioSystem;
		FMOD::System* m_lowLevelSystem;

		FMOD::System* m_2dSystem;



		static const int m_maxChannels = 32;

		int errorCheck(FMOD_RESULT result) const;

	public:
	
		void start(SystemSignal init = SystemSignal::None, ...);
		void stop(SystemSignal close = SystemSignal::None, ...);
		void update();

		inline FMOD::Studio::System& getStudioSystem() { return *m_studioSystem; }
		inline FMOD::System& getLowLevelSystem() { return *m_lowLevelSystem; }
		inline FMOD::System& get2DSystem() { return *m_2dSystem; }

	};
}
