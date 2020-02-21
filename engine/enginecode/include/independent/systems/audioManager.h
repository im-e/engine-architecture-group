#pragma once

#include "systems/system.h"
#include "fmod.hpp"
#include "fmod_studio.hpp"

namespace Engine {

	class AudioManager : public System
	{
	private:
		FMOD::Studio::System* m_studioSystem;

	};
}

