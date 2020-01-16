#pragma once

/*!
\file OpenGL_Graphics.h
\brief Graphic's context in OpenGL
*/
#include "windows/graphicsContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine
{
	/*!
	\class OpenGL_Graphics
	\brief Handles graphics management for OpenGL
	*/
	class OpenGL_Graphics : public GraphicsContext
	{
	private:
		//! Pointer to GLFW window
		GLFWwindow* m_window;
	public:
		//! Constructor
		OpenGL_Graphics(GLFWwindow* window) : m_window(window) {}

		//Inherited via GraphicsContext
		void init() override;
		void swapBuffers() override;
	};
}