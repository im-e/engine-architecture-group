#include "engine_pch.h"
#include "windows/WinPlatformWindow.h"

#include "events/Event.h"
#include "events/WindowEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseButtonEvent.h"
#include "events/MouseEvent.h"

#include "platform/GLFW_InputPoller.h"
#include "platform/GLFW_KeyCodes.h"

namespace Engine
{
	Window* Window::create(const WindowProperties& props)
	{
		return new WinPlatformWindow(props);
	}

	WinPlatformWindow::WinPlatformWindow(const WindowProperties& props)
	{
		init(props); //initialize window
	}

	void WinPlatformWindow::init(const WindowProperties & properties)
	{
		//set properties
		m_properties.m_title = properties.m_title;
		m_properties.m_width = properties.m_width;
		m_properties.m_height = properties.m_height;
		m_properties.m_isVSync = properties.m_isVSync;
		m_properties.m_isFullScreen = properties.m_isFullScreen;

		//create window
		m_window = glfwCreateWindow(properties.m_width, properties.m_height, properties.m_title.c_str(), NULL, NULL);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//create graphics context for MS Windows
		m_context = std::shared_ptr<GraphicsContext>(new OpenGL_Graphics(m_window));
		m_context->init();

		//Set user pointer for event callback
		glfwSetWindowUserPointer(m_window, &m_callback);
		setVSync(true); //turn vsync on

		LogWarn("Initialized GLFW window");

		//GLFW events callbacks

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow *fnWindow) //close callback
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(fnWindow);
			WindowCloseEvent close;
			callback(close);
		});

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* fnWindow, int fnWidth, int fnHeight) //resize callback
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(fnWindow);
			WindowResizeEvent e(fnWidth, fnHeight);
			callback(e);
		});

		glfwSetWindowFocusCallback(m_window, [](GLFWwindow *fnWindow, int fnX) //focus/lost focus callback
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(fnWindow);
			WindowLostFocusEvent lostFocus;
			callback(lostFocus);
		});

		glfwSetWindowPosCallback(m_window, [](GLFWwindow *fnWindow, int fnX, int fnY) //window position callback
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(fnWindow);
			WindowMovedEvent moved;
			callback(moved);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* fnWindow, int fnKey, int fnScanCode, int fnAction, int fnMod) //window key callback
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(fnWindow);

			switch (fnAction)
			{
				case GLFW_PRESS:
				{
					KeyDownEvent keyDown(fnKey);
					callback(keyDown);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyHoldEvent keyHold(fnKey, 1);
					callback(keyHold);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyUpEvent keyUp(fnKey);
					callback(keyUp);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* fnWindow, int fnButton, int fnAction, int fnMod) //window mouse button callback
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(fnWindow);

			switch (fnAction)
			{
				case GLFW_PRESS:
				{
					MouseButtonDownEvent buttonPress(fnButton);
					callback(buttonPress);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonUpEvent buttonReleased(fnButton);
					callback(buttonReleased);
					break;
				}

			}
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* fnWindow, double fnX, double fnY) //window cursor position callback
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(fnWindow);
			MouseMovedEvent mouseMoved(fnX, fnY);
			callback(mouseMoved);
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* fnWindow, double fnX, double fnY) //mouse scroll callback
		{
			std::function<void(Event&)> callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(fnWindow);
			MouseScrollEvent scroll(fnX, fnY);
			callback(scroll);
		});

		///record the aspect ratio
		m_aspectRatio = properties.m_width / properties.m_height;

		///enable scissor testing
		glScissor(0, 0, properties.m_width, properties.m_height);
		glEnable(GL_SCISSOR_TEST);
	}

	void WinPlatformWindow::close()
	{
		glfwDestroyWindow(m_window); //destroy window when closed
	}

	void WinPlatformWindow::onUpdate(float timestep)
	{
#ifdef NG_DEBUG
		NG_PROFILER_FUNCTION();
#endif

		//update window
		glfwPollEvents();
		m_context->swapBuffers();

		if(InputPoller::isKeyPressed(NG_KEY_ESCAPE))
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void WinPlatformWindow::onResize(unsigned int width, unsigned int height)
	{		
		float changedRatio = (float)width / (float)height; //calculate new ratio

		//set new width and height to window properties
		m_properties.m_width = width;
		m_properties.m_height = height;

		if (changedRatio < m_aspectRatio) //letterbox
		{
			float viewportY = (float)width / m_aspectRatio; //get Y axis value
			float gap = ((float)height - viewportY) / 2.0f; //calculate how big the gap should be
			
			//get current color
			GLfloat currentColor[4];
			glGetFloatv(GL_COLOR_CLEAR_VALUE, currentColor);

			glClearColor(0.f, 0.f, 0.f, 1.0f); //set the background to be complete black
			glClear(GL_COLOR_BUFFER_BIT);

			glClearColor(currentColor[0], currentColor[1], currentColor[2], currentColor[3]); //reset clear color

			//set viewport and gaps
			glViewport(0, (int)gap, width, (int)viewportY);
			glScissor(0, (int)gap, width, (int)viewportY);
		}
		else if (changedRatio > m_aspectRatio) //pillarbox
		{
			float viewportX = (float)height / m_aspectRatio; //get X axis value
			float gap = ((float)width - viewportX) / 2.f; //calculate how big the gap should be

			//get current color
			GLfloat currentColor[4];
			glGetFloatv(GL_COLOR_CLEAR_VALUE, currentColor);

			glClearColor(0.f, 0.f, 0.f, 1.0f); //set the background to be complete black
			glClear(GL_COLOR_BUFFER_BIT);

			glClearColor(currentColor[0], currentColor[1], currentColor[2], currentColor[3]); //reset clear color

			//set viewport and gaps
			glViewport((int)gap, 0, (int)viewportX, height);
			glScissor((int)gap, 0, (int)viewportX, height);
		}
		else //if it's even (resized back to the original size)
		{
			glViewport(0, 0, width, height);
		}
	}

	void WinPlatformWindow::setVSync(bool VSync)
	{
		if (VSync == true)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_properties.m_isVSync = VSync;
	}

	void WinPlatformWindow::setEventCallback(const std::function<void(Event&)>& callback)
	{
		m_callback = callback;
	}

	unsigned int WinPlatformWindow::getWidth() const
	{
		return m_properties.m_width;
	}

	unsigned int WinPlatformWindow::getHeight() const
	{
		return m_properties.m_height;
	}

	void * WinPlatformWindow::getNativeWindow() const
	{
		return m_window;
	}

	bool WinPlatformWindow::isFullScreenMode() const
	{
		return m_properties.m_isFullScreen;
	}

	bool WinPlatformWindow::isVSync() const
	{
		return m_properties.m_isVSync;
	}

	void WinPlatformWindow::setHeight(float h)
	{
		m_properties.m_height = h;
	}

	void WinPlatformWindow::setWidth(float w)
	{
		m_properties.m_width = w;
	}
}