#include "engine_pch.h"
#include "platform/OpenGL/GLFWWindowsSys.h"

namespace Engine
{
#ifdef NG_PLATFORM_WINDOWS
	InputPoller* InputPoller::m_instance = new GLFWInputPoller();
#endif

	void GLFWWindowsSys::start(SystemSignal init, ...)
	{
		if (glfwInit() == false)
		{
			LogError("Couldn't initialize GLFW!");
			return;
		}

		LogInfo("initialized windows");		
	}

	void GLFWWindowsSys::stop(SystemSignal close, ...)
	{
		glfwTerminate();
	}

}