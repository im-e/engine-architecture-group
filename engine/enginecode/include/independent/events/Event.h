#pragma once

/*!
\file Event.h
\brief Base for all events happening in the runtime
*/

/*! \def Bit(i)
	\brief Shorthand for bitshifting
	\param i 2 to the power of i
*/
#define Bit(i) 1 << i

#include "engine_pch.h"

namespace Engine
{
	/*! \enum EventType
	\brief List of all events 
	*/
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyDown, KeyHold, KeyUp,
		MouseButtonDown, MouseButtonUp, MouseMove, MouseScroll
	};

	/*! \enum EventCategory 
	\brief Event categories */
	enum EventCategory
	{
		None = 0,
		EventCategoryWindow = Bit(0),
		EventCategoryInput = Bit(1),
		EventCategoryKeyboard = Bit(2),
		EventCategoryMouse = Bit(3),
		EventCategoryMouseButton = Bit(4)
	};

	/*!\class Event 
	\brief Base event class */
	class Event
	{
	private:
		bool m_eventHandled = false; //!< Returns true if the event's been handled properly
	public:
		virtual EventType getEventType() const = 0; //!< Get event type
		virtual int getCategory() const = 0; //!< Get event category
		inline bool eventHandled() const { return m_eventHandled; } //!< Returns true if the event's been handled properly
		inline void handleEvent(bool isHandled) { m_eventHandled = isHandled; } //!< Handles events
		inline bool isInCategory(EventCategory cat) { return getCategory()& cat; } //!< Is the event in the category?
	};

	/*! \class EventDispatch
	\brief Dispatches events in the runtime 
	*/
	class EventDispatch
	{
		
		template<typename T>
		using EventFunc = std::function<bool(T&)>; //!< Creates a function that returns a boolean

	private:
		Event& m_event; //!< Event to be dispatched

	public:
		EventDispatch(Event& e) : m_event(e) {} //!< Default constructor. \param e Dispatched event

		//! Dispatches events. \param func Generic type of event (checked in the runtime) \return true if event's been handled properly
		template<typename T>
		bool dispatch(EventFunc<T> func)
		{
			if (m_event.getEventType() == T::getStaticType())
			{
				m_event.handleEvent(func(*((T*)&m_event)));
				return true;
			}
			return false;
		}
	};
}