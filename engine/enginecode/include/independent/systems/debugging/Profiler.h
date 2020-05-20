#pragma once

/*! \file Profiler.h
\brief Used for profiling functions

	Created and adapted from The Cherno YT channel - episodes about profiling
*/

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

namespace Engine
{

	/*! \struct VisualProfileResult 
	\brief Result of visual profiling
	*/
	struct VisualProfileResult
	{
		std::string m_name; //!< Name to be displayed
		long long m_start; //!< Time start point
		long long m_end; //!< Time end point
		uint32_t m_threadID; //!< Thread ID
	};

	/*! \struct InstrumentationSession 
	\brief Name of the session
	*/
	struct InstrumentationSession
	{
		std::string m_name; //!< Name
	};

	/*! \class VisualProfiler
	\brief Provides capabilities of profiling the engine
	
		Profiling results are output to json file and can be investigated using e.g chrome://tracing
	*/
	class VisualProfiler
	{
	private:
		InstrumentationSession* m_currentSession; //!< Pointer to current profiling session
		std::ofstream m_outputStream; //!< Output stream to the file
		int m_profileCount; //!< Number of profiles to be investigated
	public:
		//! Constructor
		VisualProfiler()
			: m_currentSession(nullptr), m_profileCount(0)
		{
		}

		//! Begins profiling session \param name name of the session \param filepath path to save the file
		void beginSession(const std::string& name, const std::string& filepath = "visualProfile.json")
		{
			m_outputStream.open(filepath);
			writeHeader();
			m_currentSession = new InstrumentationSession{ name };
		}

		//! Runs when session ends
		void endSession()
		{
			writeFooter();
			m_outputStream.close();
			delete m_currentSession;
			m_currentSession = nullptr;
			m_profileCount = 0;
		}

		//! Writes profile \param result profiling result
		void writeProfile(const VisualProfileResult& result)
		{
			if (m_profileCount++ > 0)
				m_outputStream << ",";

			std::string name = result.m_name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_outputStream << "{";
			m_outputStream << "\"cat\":\"function\",";
			m_outputStream << "\"dur\":" << (result.m_end - result.m_start) << ',';
			m_outputStream << "\"name\":\"" << name << "\",";
			m_outputStream << "\"ph\":\"X\",";
			m_outputStream << "\"pid\":0,";
			m_outputStream << "\"tid\":" << result.m_threadID << ",";
			m_outputStream << "\"ts\":" << result.m_start;
			m_outputStream << "}";

			m_outputStream.flush();
		}

		//! Writes beginning of the file
		void writeHeader()
		{
			m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_outputStream.flush();
		}

		//! Writes end of the file
		void writeFooter()
		{
			m_outputStream << "]}";
			m_outputStream.flush();
		}

		//! Get an instance to a profiler \return instance
		static VisualProfiler& getInstance()
		{
			static VisualProfiler instance;
			return instance;
		}
	};

	/*! \class VisualProfilerTimer
	\brief Provides capabilities to measure time in visual profiling
	*/
	class VisualProfilerTimer
	{
	public:
		//! Constructor \param name name of the profiled object
		VisualProfilerTimer(const char* name)
			: m_name(name), m_stopped(false)
		{
			m_startTimepoint = std::chrono::high_resolution_clock::now();
		}

		//! Destructor
		~VisualProfilerTimer()
		{
			if (!m_stopped)
				stop();
		}

		//! Runs when class is being deleted - marks end time point
		void stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			VisualProfiler::getInstance().writeProfile({ m_name, start, end, threadID });

			m_stopped = true;
		}

	private:
		const char* m_name; //!< Name of the profiled object
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint; //!< Start time point
		bool m_stopped; //!< Has the class already stopped?
	};
}

#ifdef NG_DEBUG

	//! \def NG_PROFILER_BEGIN_SESSION marks beginning of the profiling session
	#define NG_PROFILER_BEGIN_SESSION(name, filepath) ::Engine::VisualProfiler::getInstance().beginSession(name, filepath)
	//! \def NG_PROFILER_END_SESSION marks end of the profiling session
	#define NG_PROFILER_END_SESSION() ::Engine::VisualProfiler::getInstance().endSession()
	//! \def NG_PROFILER_SCOPE indicates that a scope will be profiled
	#define NG_PROFILER_SCOPE(name) ::Engine::VisualProfilerTimer timer##__LINE__(name);
	//! \def NG_PROFILER_FUNCTION indicates a function to be profiled
	#define NG_PROFILER_FUNCTION() NG_PROFILER_SCOPE(__FUNCSIG__)

#else
#define NG_PROFILER_BEGIN_SESSION(name, filepath)
#define NG_PROFILER_END_SESSION()
#define NG_PROFILER_SCOPE(name)
#define NG_PROFILER_FUNCTION() 
#endif