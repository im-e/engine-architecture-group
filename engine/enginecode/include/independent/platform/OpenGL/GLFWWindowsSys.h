#pragma once

/*! \file GLFWWindowsSys.h
\brief Implementation of GLFW for Windows
*/

#include "platform/WindowsSys.h"
#include "windows/WinPlatformWindow.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW_InputPoller.h"
#endif

namespace Engine
{
	/*!
	\class GLFWWindowsSys
	\brief Windows system for GLFW implementation
	*/
	class GLFWWindowsSys : public WindowsSys
	{
	public:
		//! Default constructor
		GLFWWindowsSys() {}
		void stop(SystemSignal close, ...) override;
		void start(SystemSignal init, ...) override;
	};

}
