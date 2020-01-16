/*! \file system.h
\brief Base for all the systems
*/

#pragma once

#include <cstdarg>

namespace Engine 
{
	//! Collection of system signals
	enum class SystemSignal { None = 0, Windows = 1 };

	/*! \class System
	\brief Base interface class for all engine systems
	*/
	class System
	{
	public:
		virtual ~System() {};
		virtual void start(SystemSignal init = SystemSignal::None, ...) = 0; //!< Start the system
		virtual void stop(SystemSignal close = SystemSignal::None, ...) = 0; //!< Stop the system
	};
}
