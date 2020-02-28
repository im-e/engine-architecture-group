#pragma once

/*!
	\file WinPlatformWindow.h
	\brief Handles windows on Windows systems
*/
#include "systems/Log.h"
#include "window.h"
#include "platform/OpenGL/OpenGL_Graphics.h"


namespace Engine
{
	/*!
		\class WinPlatformWindow
		\brief Window for windows system implementation
	*/
	class WinPlatformWindow : public Window
	{
	private:
		GLFWwindow* m_window; //!< OpenGL window

		float m_aspectRatio; //!< Default viewport resolution
	public:
		WinPlatformWindow(const WindowProperties& props); //!< Constructor

		// Inherited via Window
		void init(const WindowProperties & properties) override;
		void close() override;
		void onUpdate(float timestep) override;
		void onResize(unsigned int width, unsigned int height) override;
		void setVSync(bool VSync) override;
		void setEventCallback(const std::function<void(Event&)>& callback) override;
		unsigned int getWidth() const override;
		unsigned int getHeight() const override;
		void * getNativeWindow() const override;
		bool isFullScreenMode() const override;
		bool isVSync() const override;
		void setHeight(float h) override;
		void setWidth(float w) override;
		void setCursorDisabled() override;
		void setCursorEnabled() override;
	};
}