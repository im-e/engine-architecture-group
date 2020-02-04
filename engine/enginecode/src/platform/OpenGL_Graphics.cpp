#include "engine_pch.h"
#include "platform/OpenGL/OpenGL_Graphics.h"
#pragma comment(lib, "opengl32.lib")

namespace Engine
{
	void OpenGL_Graphics::init()
	{
		m_gpuAccessMutex.lock();

		glfwMakeContextCurrent(m_window);
		int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		m_initialThreadID = std::this_thread::get_id();
		m_originalContext = wglGetCurrentContext();
	}

	void OpenGL_Graphics::swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	void OpenGL_Graphics::unbindCurrentThread()
	{
		if (std::this_thread::get_id() != m_initialThreadID) {
			auto hglrc = wglGetCurrentContext();
			if (!wglDeleteContext(hglrc)) {
				auto what = GetLastError();
				LogError("Thread Unbind Failed: {0}", what);
			}
		}
		else {
			wglMakeCurrent(NULL, NULL);
		}

		m_gpuAccessMutex.unlock();
	}

	void OpenGL_Graphics::swapToCurrentThread()
	{
		m_gpuAccessMutex.lock();
		
		if (std::this_thread::get_id() != m_initialThreadID) {
			auto hwnd = glfwGetWin32Window(m_window);
			auto hdc = GetDC(hwnd);

			auto hglrc = wglCreateContext(hdc);

			if (!wglShareLists(m_originalContext, hglrc)) {
				auto what = GetLastError();
				LogError("wglShareLists Failed: {0}", what);
			}

			if (!wglMakeCurrent(hdc, hglrc)) {
				auto what = GetLastError();
				LogError("Thread Swap Failed: {0}", what);
			}
		}
		else {
			auto hwnd = glfwGetWin32Window(m_window);
			auto hdc = GetDC(hwnd);
			
			wglMakeCurrent(hdc, m_originalContext);
		}
	}
}