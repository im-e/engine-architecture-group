#pragma once

/*! \file MouseEvent.h
\brief Handles mouse events
*/
#include "Event.h"

namespace Engine
{
	/*! \class MouseEvent
	\brief Base class for all mouse events
	*/
	class MouseEvent : public Event
	{
	protected:
		float m_xOffset; //!< Value of offset on X axis
		float m_yOffset; //!< Value of offset on Y axis
	public:
		int getCategory() const override { return EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse; } //!< Returns events categories

		inline float getXOffset() { return m_xOffset; } //!< Returns x offset
		inline float getYOffset() { return m_yOffset; } //!< Returns y offset
	};

	/*! \class MouseMovedEvent
	\brief Handles events on moving mouse 
	*/
	class MouseMovedEvent : public MouseEvent
	{
	public:
		MouseMovedEvent(float x, float y) { m_xOffset = x; m_yOffset = y; } //!< Constructor. \param x Offset on horizontal axis \param y Offset on vertical axis
		EventType getEventType() const override { return EventType::MouseMove; } //!< Returns event type
		static EventType getStaticType() { return EventType::MouseMove; } //!< Returns static event type (used to check types in the runtime)
	};

	/*! \class MouseScrollEvent
	\brief Handles events on scrolling mouse
	*/
	class MouseScrollEvent : public MouseEvent
	{
	public:
		MouseScrollEvent(float x, float y) { m_xOffset = x; m_yOffset = y; } //!< Constructor. \param x Offset on horizontal axis \param y Offset on vertical axis
		EventType getEventType() const override { return EventType::MouseScroll; } //!< Returns event type
		static EventType getStaticType() { return EventType::MouseScroll; } //!< Returns static event type (used to check types in the runtime)
	};
}