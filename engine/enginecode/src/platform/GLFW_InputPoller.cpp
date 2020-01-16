#include "engine_pch.h"
#include "platform/GLFW_InputPoller.h"
#include "core/application.h"
#include "windows/WinPlatformWindow.h"

#include "platform/GLFW_KeyCodes.h"

namespace Engine
{
	bool GLFWInputPoller::isKeyPressedImpl(int key)
	{
#ifdef NG_DEBUG
		NG_PROFILER_FUNCTION();
#endif
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::getInstance().getWindow()->getNativeWindow());
		int result = glfwGetKey(window, key);
		return result == NG_PRESS || result == NG_REPEAT;
	}

	bool GLFWInputPoller::isMouseButtonPressedImpl(int button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::getInstance().getWindow()->getNativeWindow());
		int result = glfwGetMouseButton(window, button);
		return result == NG_PRESS;
	}

	glm::vec2 GLFWInputPoller::getMousePositionImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::getInstance().getWindow()->getNativeWindow());
		
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		glm::vec2 mousePos;
		mousePos.x = x;
		mousePos.y = y;

		return mousePos;
	}

	float GLFWInputPoller::getMouseXImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::getInstance().getWindow()->getNativeWindow());

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return x;
	}

	float GLFWInputPoller::getMouseYImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::getInstance().getWindow()->getNativeWindow());

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return y;
	}
}