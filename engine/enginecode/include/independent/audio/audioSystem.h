#pragma once

/*! \file audioSystem.h
\brief Responsible for starting and stopping FMOD audio
*/

#include <map>
#include <string>
#include <glm/glm.hpp>
#include "systems/system.h"
#include "audio/audioRollOff.h"
#include "audio/audioGeometryDefinition.h"
#include "fmod.hpp"
#include "fmod_studio.hpp"

namespace Engine 
{
	/*! \class AudioSystem
	\brief Handles starting and stopping of audio system.
	
		Inherits from System.
	*/
	class AudioSystem : public System
	{
	private:
		FMOD::Studio::System* m_studioSystem; //!M FMOD studio system
		FMOD::System* m_lowLevelSystem; //!< FMOD low level system
		FMOD::System* m_2dSystem; //!< Fmod 2D system

		static const int m_maxChannels = 32; //!< Max number of channels

		//! Function to check for errors. \param result FMOD result \return true if error occurred
		int errorCheck(FMOD_RESULT result) const; 

	public:
		void start(SystemSignal init = SystemSignal::None, ...);
		void stop(SystemSignal close = SystemSignal::None, ...);
		//! Updates FMOD per frame
		void update();

		//! Get FMOD studio system \return studio system
		inline FMOD::Studio::System& getStudioSystem() { return *m_studioSystem; }
		//! Get FMOD low level system \return low level system
		inline FMOD::System& getLowLevelSystem() { return *m_lowLevelSystem; }
		//! Get FMOD 2D system \return 2D system
		inline FMOD::System& get2DSystem() { return *m_2dSystem; }

	};
}

