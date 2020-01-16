#pragma once

#include <gtest/gtest.h>
#include "events/Event.h"
#include "events/WindowEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseButtonEvent.h"
#include "events/MouseEvent.h"

/* Window events tests */
const int height = 1024;
const int width = 720;

Engine::WindowResizeEvent resize(width, height);
Engine::WindowCloseEvent cl;
Engine::WindowLostFocusEvent lostFocus;
Engine::WindowMovedEvent windowMoved;
/************************************************************/

/* Key events tests */
const int keyCode = 1;
const int repeatCount = 10;

Engine::KeyDownEvent keyPressed(keyCode);
Engine::KeyHoldEvent keyHold(keyCode, repeatCount);
Engine::KeyUpEvent keyUp(keyCode);
/************************************************************/

/* Mouse button events tests */
unsigned int button = 1;

Engine::MouseButtonDownEvent buttonDown(button);
Engine::MouseButtonUpEvent buttonUp(button);

/************************************************************/

/* Mouse events tests */

const float x = 5;
const float y = 6;

Engine::MouseMovedEvent mouseMoved(x, y);
Engine::MouseScrollEvent mouseScroll(x, y);

/************************************************************/

bool OnWindowEvent(Engine::WindowEvent& e)
{
	return true;
}

bool OnKeyEvent(Engine::KeyEvent& e)
{
	return true;
}

bool OnMouseButtonEvent(Engine::MouseButtonEvent& e)
{
	return true;
}

bool OnMouseEvent(Engine::MouseEvent& e)
{
	return true;
}