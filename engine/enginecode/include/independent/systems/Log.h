#pragma once
/*!
\file Log.h 
\brief Handles logging information to the screen
*/

#include <memory>
#include <stdarg.h>

#include "systems/system.h"
#include "spdlog/spdlog.h"

namespace Engine
{	/*! \class Log 
	\brief Logger class responsible for printing information the users
	*/
	static class Log : public System
	{
	private:
		static std::shared_ptr<spdlog::logger> m_logger; //!< Pointer to spdlog class

	public:
		Log(); //!< Default constructor

		// Inherited via System
		virtual void start(SystemSignal init, ...) override;
		virtual void stop(SystemSignal close, ...) override;

		inline static std::shared_ptr<spdlog::logger>& getLogger() { return m_logger; } //!< Returns logger

	};
}

/*! \def LogInfo(...)
	Displays information
*/
#define LogInfo(...) ::Engine::Log::getLogger()->info(__VA_ARGS__)
/*! \def LogWarn
	Displays warnings
*/
#define LogWarn(...) ::Engine::Log::getLogger()->warn(__VA_ARGS__)
/*! \def LogError
	Displays errors
*/
#define LogError(...) ::Engine::Log::getLogger()->error(__VA_ARGS__)