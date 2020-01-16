#pragma once

/*! \file MouseButtonEvent.h
\brief Handles events bound to mouse buttons
*/
#include "Event.h"

namespace Engine
{
	/*! \class MouseButtonEvent
	\brief Base class for all mouse button events
	*/
	class MouseButtonEvent : public Event
	{
	protected:
		int m_button; //!< Mouse button value
	public:
		int getCategory() const override { return EventCategory::EventCategoryInput | EventCategory::EventCategoryMouseButton; } //!< Returns event category

		inline int getButtonCode() { return m_button; } //!< Returns mouse button value
	};

	/*! \class MouseButtonDownEvent
	\brief Handles events on pressing a mouse button
	*/
	class MouseButtonDownEvent : public MouseButtonEvent
	{
	public:
		MouseButtonDownEvent(int button) { m_button = button; } //!< Constructor. \param button Pressed button
		EventType getEventType() const override { return EventType::MouseButtonDown; } //!< Returns event type
		static EventType getStaticType() { return EventType::MouseButtonDown; } //!< Returns static event type (used to check types in the runtime)
	};

	/*! \class MouseButtonUpEvent
	\brief Handles events on releasing a mouse button
	*/
	class MouseButtonUpEvent : public MouseButtonEvent
	{
	public:
		MouseButtonUpEvent(int button) { m_button = button; } //!< Constructor. \param button Released button
		EventType getEventType() const override { return EventType::MouseButtonUp; } //!< Returns event type
		static EventType getStaticType() { return EventType::MouseButtonUp; } //!< Returns static event type (used to check types in the runtime)
	};
}