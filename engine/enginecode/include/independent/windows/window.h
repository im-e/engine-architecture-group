#pragma once

/*!
	\file window.h
	\brief Every window's interface
*/

#include "graphicsContext.h"
#include <string>
#include <functional>
#include "events/Event.h"

namespace Engine 
{
	class Event; //Class forward declaration

	/*! \struct WindowProperties
		\brief Holds basic data about a window
	*/
	struct WindowProperties
	{
		std::string m_title; //!< Window's title
		unsigned int m_width; //!< Window's size on X axis
		unsigned int m_height; //!< Window's size on Y axis
		bool m_isFullScreen; //!< Is the window fullscreen?
		bool m_isVSync; //!< Is the vSync enabled?

		//! Struct's constructor
		WindowProperties(const std::string& title = "DMU Engine", unsigned int width = 800, unsigned int height = 600, bool fullscreen = false, bool vSync = true)
			: m_title(title), m_width(width), m_height(height), m_isFullScreen(fullscreen), m_isVSync(vSync) {}
	};

	/*!
	\class Window
	\brief Window's interface
	*/
	class Window
	{
	protected:
		std::shared_ptr<GraphicsContext> m_context; //!< OpenGL graphics context
		 
		WindowProperties m_properties; //!< Windows properties struct object
	public:
		virtual void init(const WindowProperties& properties) = 0; //!< Initialize the window
		virtual void close() = 0; //!< Shut the window down

		virtual void onUpdate(float timestep) = 0; //!< Updates window 
		virtual void onResize(unsigned int width, unsigned int height) = 0; //!< Called when the window is being resized
		virtual void setVSync(bool VSync) = 0; //!< Turn vSync on/off
		virtual void setEventCallback(const std::function<void(Event&)>& callback) = 0; //!< Sets event callbacks
		virtual unsigned int getWidth() const = 0; //!< Get window's size on x axis. \return m_properties.m_width
		virtual unsigned int getHeight() const = 0; //!< Get window's size on Y axis. \return m_properties.m_height
		virtual void* getNativeWindow() const = 0; //!< Get native window
		virtual bool isFullScreenMode() const = 0; //!< Is the window in the fullscreen mode?
		virtual bool isVSync() const = 0; //!< Is the window vSynched?
		virtual void setHeight(float h) = 0; //!< Sets window's size on x axis \param h new height
		virtual void setWidth(float w) = 0; //!< Sets window's size on y axis \param w new width

		std::function<void(Event&)> m_callback; //!< Callback event function
		static Window* create(const WindowProperties& properties = WindowProperties()); //!< Creates window
		std::shared_ptr<GraphicsContext> getGraphicsContext() { return m_context; } //!< stef wazz here lol

		//! Virtual destructor
		virtual ~Window() {};
	};
}

