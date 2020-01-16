#include "EventTests.h"

TEST(WindowEvents, CloseTest)
{
	Engine::EventDispatch d(cl);
	d.dispatch<Engine::WindowCloseEvent>(std::bind(OnWindowEvent, std::placeholders::_1));
	bool result = cl.eventHandled();
	EXPECT_TRUE(result);
}

TEST(WindowEvents, CloseDispatch)
{
	Engine::EventDispatch d(cl);
	bool result = d.dispatch<Engine::WindowCloseEvent>(std::bind(OnWindowEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(WindowEvents, ResizeTest)
{
	int gotWidth = resize.getWidth();
	int gotHeight = resize.getHeight();
	Engine::EventType type = resize.getEventType();

	EXPECT_EQ(gotWidth, width);
	EXPECT_EQ(gotHeight, height);
}

TEST(WindowEvents, ResizeDispatch)
{
	Engine::EventDispatch d(resize);
	bool result = d.dispatch<Engine::WindowResizeEvent>(std::bind(OnWindowEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(WindowEvents, ResizeHandled)
{
	Engine::EventDispatch d(resize);
	d.dispatch<Engine::WindowResizeEvent>(std::bind(OnWindowEvent, std::placeholders::_1));
	bool result = resize.eventHandled();
	EXPECT_TRUE(result);
}

TEST(WindowEvents, LostFocusDispatch)
{
	Engine::EventDispatch d(lostFocus);
	bool result = d.dispatch<Engine::WindowLostFocusEvent>(std::bind(OnWindowEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(WindowEvents, LostFocusHandled)
{
	Engine::EventDispatch d(lostFocus);
	d.dispatch<Engine::WindowLostFocusEvent>(std::bind(OnWindowEvent, std::placeholders::_1));
	bool result = lostFocus.eventHandled();
	EXPECT_TRUE(result);
}

TEST(WindowEvents, WindowMovedHandled)
{
	Engine::EventDispatch d(windowMoved);
	d.dispatch<Engine::WindowMovedEvent>(std::bind(OnWindowEvent, std::placeholders::_1));
	bool result = windowMoved.eventHandled();
	EXPECT_TRUE(result);
}

TEST(WindowEvents, WindowMovedDispatch)
{
	Engine::EventDispatch d(windowMoved);
	bool result = d.dispatch<Engine::WindowMovedEvent>(std::bind(OnWindowEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(KeyEvents, KeyPressed)
{
	int gotKeyCode = keyPressed.getKeyCode();
	Engine::EventType type = keyPressed.getEventType();

	EXPECT_EQ(gotKeyCode, keyCode);
}

TEST(KeyEvents, KeyPressedDispatch)
{
	Engine::EventDispatch d(keyPressed);
	bool result = d.dispatch<Engine::KeyDownEvent>(std::bind(OnKeyEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(KeyEvents, KeyPressedHandled)
{
	Engine::EventDispatch d(keyPressed);
	d.dispatch<Engine::KeyDownEvent>(std::bind(OnKeyEvent, std::placeholders::_1));
	bool result = keyPressed.eventHandled();
	EXPECT_TRUE(result);
}

TEST(KeyEvents, KeyHold)
{
	int gotKeyCode = keyHold.getKeyCode();
	int repeat = keyHold.getRepeatCount();
	Engine::EventType type = keyHold.getEventType();

	EXPECT_EQ(gotKeyCode, keyCode);
	EXPECT_EQ(repeat, repeatCount);
}

TEST(KeyEvents, KeyHoldDispatch)
{
	Engine::EventDispatch d(keyHold);
	bool result = d.dispatch<Engine::KeyHoldEvent>(std::bind(OnKeyEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(KeyEvents, KeyHoldHandled)
{
	Engine::EventDispatch d(keyHold);
	d.dispatch<Engine::KeyHoldEvent>(std::bind(OnKeyEvent, std::placeholders::_1));
	bool result = keyHold.eventHandled();
	EXPECT_TRUE(result);
}

TEST(KeyEvents, KeyReleased)
{
	int gotKeyCode = keyUp.getKeyCode();
	Engine::EventType type = keyUp.getEventType();

	EXPECT_EQ(gotKeyCode, keyCode);
}

TEST(KeyEvents, KeyReleaseDispatch)
{
	Engine::EventDispatch d(keyUp);
	bool result = d.dispatch<Engine::KeyUpEvent>(std::bind(OnKeyEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(KeyEvents, KeyReleaseHandled)
{
	Engine::EventDispatch d(keyUp);
	d.dispatch<Engine::KeyUpEvent>(std::bind(OnKeyEvent, std::placeholders::_1));
	bool result = keyUp.eventHandled();
	EXPECT_TRUE(result);
}

TEST(MouseButtonEvents, MouseButtonDownEvent)
{
	int b = buttonDown.getButtonCode();
	Engine::EventType type = buttonDown.getEventType();

	EXPECT_EQ(b, button);
}

TEST(MouseButtonEvents, MouseButtonDownDispatch)
{
	Engine::EventDispatch d(buttonDown);
	bool result = d.dispatch<Engine::MouseButtonDownEvent>(std::bind(OnMouseButtonEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(MouseButtonEvents, MouseButtonDownHandled)
{
	Engine::EventDispatch d(buttonDown);
	d.dispatch<Engine::MouseButtonDownEvent>(std::bind(OnMouseButtonEvent, std::placeholders::_1));
	bool result = buttonDown.eventHandled();
	EXPECT_TRUE(result);
}

TEST(MouseButtonEvents, MouseButtonUpEvent)
{
	int b = buttonUp.getButtonCode();
	Engine::EventType type = buttonUp.getEventType();

	EXPECT_EQ(b, button);
}

TEST(MouseButtonEvents, MouseButtonUpDispatch)
{
	Engine::EventDispatch d(buttonUp);
	bool result = d.dispatch<Engine::MouseButtonUpEvent>(std::bind(OnMouseButtonEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(MouseButtonEvents, MouseButtonUpHandled)
{
	Engine::EventDispatch d(buttonUp);
	d.dispatch<Engine::MouseButtonUpEvent>(std::bind(OnMouseButtonEvent, std::placeholders::_1));
	bool result = buttonUp.eventHandled();
	EXPECT_TRUE(result);
}

TEST(MouseEvents, MouseMoved)
{
	float getX = mouseMoved.getXOffset();
	float getY = mouseMoved.getYOffset();
	Engine::EventType type = mouseMoved.getEventType();

	EXPECT_EQ(x, getX);
	EXPECT_EQ(y, getY);
}

TEST(MouseEvents, MouseMovedDispatch)
{
	Engine::EventDispatch d(mouseMoved);
	bool result = d.dispatch<Engine::MouseMovedEvent>(std::bind(OnMouseEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(MouseEvents, MouseMovedHandled)
{
	Engine::EventDispatch d(mouseMoved);
	d.dispatch<Engine::MouseMovedEvent>(std::bind(OnMouseEvent, std::placeholders::_1));
	bool result = mouseMoved.eventHandled();
	EXPECT_TRUE(result);
}

TEST(MouseEvents, MouseScrolled)
{
	float getX = mouseScroll.getXOffset();
	float getY = mouseScroll.getYOffset();
	Engine::EventType type = mouseScroll.getEventType();

	EXPECT_EQ(x, getX);
	EXPECT_EQ(y, getY);
}

TEST(MouseEvents, MouseScrollDispatch)
{
	Engine::EventDispatch d(mouseScroll);
	bool result = d.dispatch<Engine::MouseScrollEvent>(std::bind(OnMouseEvent, std::placeholders::_1));
	EXPECT_TRUE(result);
}

TEST(MouseEvents, MouseScrollHandled)
{
	Engine::EventDispatch d(mouseScroll);
	d.dispatch<Engine::MouseScrollEvent>(std::bind(OnMouseEvent, std::placeholders::_1));
	bool result = mouseScroll.eventHandled();
	EXPECT_TRUE(result);
}