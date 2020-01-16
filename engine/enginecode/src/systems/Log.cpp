#include "engine_pch.h"
#include "systems/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine
{
	std::shared_ptr<spdlog::logger> Log::m_logger;

	Log::Log()
	{		
	}

	void Log::start(SystemSignal init, ...)
	{
		//initialize log system
		spdlog::set_pattern("%^[%T] [%n]: %v%$");
		m_logger = spdlog::stdout_color_mt("CONSOLE");
		m_logger->set_level(spdlog::level::trace);
	}

	void Log::stop(SystemSignal close, ...)
	{
		//stop log system
		m_logger.reset(); //free the memory
	}
}

