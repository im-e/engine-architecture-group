#pragma once

/*!
\file WindowsSys.h
\brief Windows system implementation
*/

#include <memory>

#include "systems/system.h"
#include "systems/Log.h"

namespace Engine
{
	/*!
	\class WindowsSys
	\brief Basic class for all windows related systems
	*/
	class WindowsSys : public System
	{
	public:
		//Inherited via System
		virtual void start(SystemSignal init, ...) = 0;
		virtual void stop(SystemSignal close, ...) = 0;
	};
}