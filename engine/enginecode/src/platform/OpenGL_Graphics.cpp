#include "engine_pch.h"
#include "platform/OpenGL/OpenGL_Graphics.h"

namespace Engine
{
	void OpenGL_Graphics::init()
	{
		glfwMakeContextCurrent(m_window);
		int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	void OpenGL_Graphics::swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}
}