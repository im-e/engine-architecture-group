#pragma once

/*!
\file InputPoller.h
\brief Base for all input pollers
*/
#include <glm/glm.hpp>

namespace Engine
{
	/*!
	\class InputPoller
	\brief Interface for input polls
	*/
	class InputPoller
	{
	private:
		static InputPoller* m_instance; //!< Singleton instance pointer
	protected:
		virtual bool isKeyPressedImpl(int key) = 0; //!< Has a key been pressed? \param key pressed key code \return true if an ASCII key has been pressed
		virtual bool isMouseButtonPressedImpl(int button) = 0; //!< Has a mouse button been pressed? \param button pressed button code \return true if a button has been pressed
		virtual glm::vec2 getMousePositionImpl() = 0; //!< Where is the mouse? \return mouse position in glm::vec2 format 
		virtual float getMouseXImpl() = 0; //!< What is position of the mouse on the X axis? \return X mouse position
		virtual float getMouseYImpl() = 0; //!< What is position of the mouse on the Y axis? \return Y mouse position
	public:
		inline static bool isKeyPressed(int key) { return m_instance->isKeyPressedImpl(key); } //!< Has a key been pressed? \param key pressed key code \return true if an ASCII key has been pressed
		inline static bool isMouseButtonPressed(int button) { return m_instance->isMouseButtonPressedImpl(button); } //!< Has a mouse button been pressed? \param button pressed button code \return true if a button has been pressed
		inline static glm::vec2 getMousePosition() { return m_instance->getMousePositionImpl(); } //!< Where is the mouse? \return mouse position in glm::vec2 format 
		inline static float getMouseX() { return m_instance->getMouseXImpl(); } //!< What is position of the mouse on the X axis? \return X mouse position
		inline static float getMouseY() { return m_instance->getMouseYImpl(); } //!< What is position of the mouse on the Y axis? \return Y mouse position
		inline static InputPoller* getInstance() { return m_instance; } //!< Returns class instance \return pointer to the InputPoller singleton 

		~InputPoller() { delete m_instance; } //!< Destructor
	};
}