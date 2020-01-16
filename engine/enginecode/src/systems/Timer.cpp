#include "engine_pch.h"
#include "systems/Timer.h"

namespace Engine
{
	Timer::Timer()
	{
	}

	void Timer::start(SystemSignal init, ...)
	{
		m_timeStart = Time_Now; //record current time on init
	}

	void Timer::stop(SystemSignal close, ...)
	{
	}

	std::chrono::high_resolution_clock::time_point Timer::currentTime()
	{
		return std::chrono::high_resolution_clock::now(); //get current time
	}

	float Timer::getFPS(std::chrono::high_resolution_clock::time_point frameStart, std::chrono::high_resolution_clock::time_point frameEnd)
	{
		m_frameDuration = frameEnd - frameStart; //calculate how long a frame is

		float timestep = m_frameDuration.count(); //calculate timestep
		float fps = 1.0f / timestep; //calculate fps

		return fps;
	}

	float Timer::measureTime(std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point end)
	{
		//calculate how much time has elapsed
		std::chrono::duration<float> elapsedTime = end - start;

		float time = elapsedTime.count(); //get elapsed time
		return time;
	}

	float Timer::getTimeSeconds()
	{
		//get time in seconds since program startup
		std::chrono::duration<float> timePassed = Time_Now - m_timeStart;
		float currentTime = timePassed.count();
		return currentTime;
	}

	float Timer::getTimeMilliseconds()
	{
		//get time in milliseconds since program startup
		std::chrono::duration<float, std::milli> timePassed = Time_Now - m_timeStart;
		float currentTime = timePassed.count();
		return currentTime;
	}

	Timer::~Timer()
	{
	}
}
