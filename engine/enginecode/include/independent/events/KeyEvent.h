#pragma once

/*! \file KeyEvent.h
\brief Handles events on keyboard input 
*/

#include "Event.h"

namespace Engine
{
	/*! \class KeyEvent
	\brief Base class for keyboard input events
	*/
	class KeyEvent : public Event
	{
	protected:
		int m_keyCode; //!< Pressed key
	public:
		KeyEvent() {} //!< Default constructor
		int getCategory() const override { return EventCategory::EventCategoryInput | EventCategory::EventCategoryKeyboard; } //!< Get event's category
		inline int getKeyCode() { return m_keyCode; } //!< Returns pressed key
	};

	/*! \class KeyDownEvent
	\brief Handles events happening after pressing a key
	*/
	class KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(int key) { m_keyCode = key; } //!< Constructor. \param key Pressed key
		EventType getEventType() const override { return EventType::KeyDown; } //!< Returns event type
		static EventType getStaticType() { return EventType::KeyDown; } //!< Returns static event type (used to check types in the runtime)
	};

	/*! \class KeyHoldEvent
	\brief Handles events happening when key is held
	*/
	class KeyHoldEvent : public KeyEvent
	{
	private:
		int m_repeatCount; //!< Number of key repeats
	public:
		KeyHoldEvent(int key, int repeat) : m_repeatCount(repeat) { m_keyCode = key; } //!< Constructor. \param key Held key \param repeat Number of repeats
		EventType getEventType() const override { return EventType::KeyHold; } //!< Returns event type
		static EventType getStaticType() { return EventType::KeyHold; } //!< Returns static event type (used to check types in the runtime)

		inline int getRepeatCount() { return m_repeatCount; } //!< Returns number of repeats
	};

	/*! \class KeyUpEvent
	\brief Handles events happening when key is released
	*/
	class KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(int key) { m_keyCode = key; } //!< Constructor. \param key Released key
		EventType getEventType() const override { return EventType::KeyUp; } //!< Returns event type
		static EventType getStaticType() { return EventType::KeyUp; } //!< Returns static event type (used to check types in the runtime)
	};
}