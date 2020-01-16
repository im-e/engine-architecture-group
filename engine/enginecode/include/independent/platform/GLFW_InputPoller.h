#pragma once

/*!
\file GLFW_InputPoller.h
\brief Implements input polling for GLFW
*/

#include "InputPoller.h"

namespace Engine
{
	/*!
	\class GLFWInputPoller
	\brief Handles input polling for GLFW context
	*/
	class GLFWInputPoller : public InputPoller
	{
	protected:
		// Inherited via InputPoller
		virtual bool isKeyPressedImpl(int key) override;
		virtual bool isMouseButtonPressedImpl(int button) override;
		virtual glm::vec2 getMousePositionImpl() override;
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
	};
}