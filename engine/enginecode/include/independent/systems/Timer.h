#pragma once

/*!
\file Timer.h
\brief Responsible for measuring time
*/

#include "systems/system.h"
#include <memory>
#include <chrono>

namespace Engine
{
	/*! \class Timer 
	\brief Handles measuring time 
	*/
	static class Timer : public System
	{
	private:
		std::chrono::duration<float> m_frameDuration; //!< Actual frame duration
		std::chrono::high_resolution_clock::time_point m_timeStart; //!< Start point for time measuring
	public:
		Timer(); //!< Default constructor

		// Inherited via System
		virtual void start(SystemSignal init, ...) override;
		virtual void stop(SystemSignal close, ...) override;

		static std::chrono::high_resolution_clock::time_point currentTime(); //!< Get current time
		float getFPS(std::chrono::high_resolution_clock::time_point frameStart, std::chrono::high_resolution_clock::time_point frameEnd); //!< Count frames per second
		float measureTime(std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point end); //!< Measure elapsed time in seconds
		float getTimeSeconds(); //!< Returns time in seconds since the start of the application
		float getTimeMilliseconds(); //!< Returns time in milliseconds since the start of the application

		//! Destructor
		~Timer();
	};
}

//! \def Time_Now shorthand for getting current time in the application
#define Time_Now ::Engine::Timer::currentTime()
//! \def Time_Point shorthand for marking time points
#define Time_Point std::chrono::high_resolution_clock::time_point