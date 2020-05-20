#pragma once

/*! \file audioSound.h
\brief Definition of sounds
*/
#include <string>
#include <glm/glm.hpp>
#include "fmod.hpp"
#include "fmod.hpp"
#include "audio/audioRollOff.h"

namespace Engine
{
	/*! \class Sound
	\brief Abstracts an FMOD sound
	*/
	class Sound
	{
	private:
		FMOD::Sound* m_sound; //!< FMOD sound

		std::string m_name; //!< Name of the sound
		bool m_b3d; //!< Is sound 3D?
		bool m_Looping; //!< Is sound looping?
		bool m_Stream; //!< Is sound streamed?
		float m_minDist; //!< Minimum distance to hear sound from
		float m_maxDist; //!< Max distance sound can be heard from
		RollOff m_rollOff; //!< Roll off type

		//! Function to check for errors. \param result FMOD result \return true if error occurred
		int errorCheck(FMOD_RESULT result) const;

	public:
		/*! Constructor of sound
		\param strSoundName soundName
		\param b3d is it 3D sound?
		\param bLooping should the sound loop?
		\param bStream is sound streamed?
		\param minDist minimum distance to hear sound from
		\param maxDist maximum distance to hear sound from
		\param rollOff sound roll off type
		*/
		Sound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false, float minDist = 0.25f,
			float maxDist = 10000.f, RollOff rollOff = RollOff::InverseTapered);

		/*! Creates a sound
		\param strSoundName soundName
		\param b3d
		\param bLooping should the sound loop?
		\param bStream is sound streamed?
		\param minDist minimum distance to hear sound from
		\param maxDist maximum distance to hear sound from
		\param rollOff sound roll off type
		*/
		static Sound* create(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false, float minDist = 0.25f,
			float maxDist = 10000.f, RollOff rollOff = RollOff::InverseTapered);
		
		//! Get sound name \return sound name
		std::string getName() { return m_name; }
		//! Get whether sound is 3D \return true if sound is 3D
		bool get3D() { return m_b3d; } 
		//! Get whether sound is looping \return true if sound set to loop
		bool getLooping() { return m_Looping; } 
		//! Get whether sound is streamed \return true if streamed
		bool getStream() { return m_Stream; } 
		//! Get minimum distance sound is heard from \return minimum distance
		float getMinDist() { return m_minDist; }
		//! Get maximum distance sound is heard from \return maximum distance
		float getMaxDist() { return m_maxDist; } 
		//! Get sound Roll Off type \return Roll Off type
		RollOff getRollOff() { return m_rollOff; } 
		//! Get an FMOD sound \return FMOD sound
		FMOD::Sound* getFmodSound() { return m_sound; }
	};

}

