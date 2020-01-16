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
		virtual void init(const WindowProperties & properties) override;
		virtual void close() override;
		virtual void onUpdate(float timestep) override;
		virtual void onResize(unsigned int width, unsigned int height) override;
		virtual void setVSync(bool VSync) override;
		virtual void setEventCallback(const std::function<void(Event&)>& callback) override;
		virtual unsigned int getWidth() const override;
		virtual unsigned int getHeight() const override;
		virtual void * getNativeWindow() const override;
		virtual bool isFullScreenMode() const override;
		virtual bool isVSync() const override;
		virtual void setHeight(float h) override;
		virtual void setWidth(float w) override;

	};
}