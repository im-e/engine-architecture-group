#pragma once

/*! \file WindowEvent.h
\brief Responsible for handling window events
*/
#include "Event.h"

namespace Engine
{
	/*! \class WindowEvent
	\brief Base class for window's events 
	*/
	class WindowEvent : public Event
	{
	public:
		int getCategory() const override { return EventCategoryWindow; } //!< Returns event category
	};

	/*! \class WindowCloseEvent 
	\brief Responsible for events happening when window is closed */
	class WindowCloseEvent : public WindowEvent
	{
	private:
	public:
		WindowCloseEvent() {} //!< Default constructor
		EventType getEventType() const override { return EventType::WindowClose; } //!< Returns event's type
		static EventType getStaticType() { return EventType::WindowClose; } //!< Returns static event type (used to check types in the runtime)
	};

	/*! \class WindowResizeEvent
	\brief Responsible for events happening on window's resizing
	*/
	class WindowResizeEvent : public WindowEvent
	{
	private:
		int m_width; //!< Horizontal size of the window
		int m_height; //!< Vertical size of the window
	public:
		WindowResizeEvent(int x, int y) : m_width(x), m_height(y) {}//!< Constructor. \param x Size of the window on the X axis \param y Size of the window on the Y axis 
		EventType getEventType() const override { return EventType::WindowResize; } //!< Returns event's type
		static EventType getStaticType() { return EventType::WindowResize; } //!< Returns static event type (used to check types in the runtime)

		inline int getWidth() { return m_width; } //!< Returns window's width
		inline int getHeight() { return m_height; } //!< Returns window's height
	};

	/*! \class WindowLostFocusEvent
	\brief Responsible for events happening on window's losing focus
	*/
	class WindowLostFocusEvent : public WindowEvent
	{
	public:
		WindowLostFocusEvent() {}
		EventType getEventType() const override { return EventType::WindowLostFocus; } //!< Returns event's type
		static EventType getStaticType() { return EventType::WindowLostFocus; } //!< Returns static event type (used to check types in the runtime)
	};

	/*! \class WindowMovedEvent
	\brief Responsible for events happening on window's being moved
	*/
	class WindowMovedEvent : public WindowEvent
	{
	private:
	public:
		WindowMovedEvent() {} //!< Default constructor
		EventType getEventType() const override { return EventType::WindowMoved; } //!< Returns event's type
		static EventType getStaticType() { return EventType::WindowMoved; } //!< Returns static event type (used to check types in the runtime)
	};
}