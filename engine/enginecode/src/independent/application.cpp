/*! \file application.cpp
*/

#include "engine_pch.h"
#include "core/application.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/OpenGL/GLFWWindowsSys.h"
#include "windows/WinPlatformWindow.h"
#include "platform/GLFW_KeyCodes.h"
#endif

#include "data/json/JsonLayer.h"

namespace Engine 
{
	Application* Application::s_instance = nullptr;

	//Hi
	
	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		//create logger
		m_logger = std::make_shared<Log>();
		m_logger->start(Engine::SystemSignal::None);

		//create timer
		m_timer = std::make_shared<Timer>();
		m_timer->start(Engine::SystemSignal::None);

		m_physWorld = std::make_shared<Physics>();
		m_physWorld->start(Engine::SystemSignal::None);

		// Create layer stack
		m_layerStack.reset(new LayerStack());
		m_layerStack->start(SystemSignal::None);

		m_imGui = std::make_shared<ImGuiSystem>();
		m_imGui->start(SystemSignal::Windows);

		lua = luaL_newstate(); // init Lua
		luaL_openlibs(lua);

		//initialize windows system
#ifdef NG_PLATFORM_WINDOWS
		m_windowsSystem = std::shared_ptr<WindowsSys>(new GLFWWindowsSys());
#endif
		m_windowsSystem->start(Engine::SystemSignal::Windows);

		m_appWindow = std::unique_ptr<Window>(Window::create());
		m_appWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

	}

	void Application::run()
	{
		Time_Point frameStart, frameEnd;

		m_timestep = 0.f;

		while (m_running)
		{
			//count fps
			frameStart = Time_Now;

			m_physWorld->getWorld()->update(m_timestep);

			for (auto& it = m_layerStack->begin(); it != m_layerStack->end(); it++)
			{
				(*it)->onUpdate(m_timestep);
			}

			m_appWindow->onUpdate(m_timestep);

			frameEnd = Time_Now;

			m_timestep = m_timer->measureTime(frameStart, frameEnd);
			m_fps = m_timer->getFPS(frameStart, frameEnd);

			//LogInfo("FPS: {0}", m_fps);
			//LogWarn("Timestep: {0}", m_timestep);
		}
	}

	lua_State * Application::getLuaState()
	{
		return lua;
	}

	std::shared_ptr<Physics>& Application::getPhysics()
	{
		return m_physWorld;
	}

	void Application::onEvent(Event & e)
	{
		EventDispatch dispatcher(e);

		//window events
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::onResize, this, std::placeholders::_1));
		dispatcher.dispatch<WindowLostFocusEvent>(std::bind(&Application::onLostFocus, this, std::placeholders::_1));

		//Keyboard input events
		dispatcher.dispatch<KeyDownEvent>(std::bind(&Application::onKeyDown, this, std::placeholders::_1));
		dispatcher.dispatch<KeyHoldEvent>(std::bind(&Application::onKeyHold, this, std::placeholders::_1));
		dispatcher.dispatch<KeyUpEvent>(std::bind(&Application::onKeyUp, this, std::placeholders::_1));

		//Mouse button events
		dispatcher.dispatch<MouseButtonDownEvent>(std::bind(&Application::onMouseButtonDown, this, std::placeholders::_1));
		dispatcher.dispatch<MouseButtonUpEvent>(std::bind(&Application::onMouseButtonUp, this, std::placeholders::_1));

		//Mouse events
		dispatcher.dispatch<MouseMovedEvent>(std::bind(&Application::onMouseMove, this, std::placeholders::_1));
		dispatcher.dispatch<MouseScrollEvent>(std::bind(&Application::onMouseScroll, this, std::placeholders::_1));

		for (auto it = m_layerStack->end(); it != m_layerStack->begin();)
		{
			(*--it)->onEvent(e);

			if (e.eventHandled())
				break;
		}
	}

	bool Application::onClose(WindowCloseEvent & e)
	{
		LogError("Closing...");
		m_running = false;
		return true;
	}

	bool Application::onResize(WindowResizeEvent & e)
	{
		m_appWindow->setHeight(e.getHeight());
		m_appWindow->setWidth(e.getWidth());

		m_appWindow->onResize(e.getWidth(), e.getHeight());	
		return true;
	}

	bool Application::onLostFocus(WindowLostFocusEvent & e)
	{
		//LogWarn("Lost focus on window");
		return true;
	}

	bool Application::onWindowMoved(WindowMovedEvent & e)
	{
		//LogInfo("Window moved");
		return true;
	}

	int Application::onKeyDown(KeyDownEvent & e)
	{
		return e.getKeyCode();
	}

	int Application::onKeyHold(KeyHoldEvent & e)
	{
		//LogInfo("Hold key: {0}, repeated {1} times.", (char)e.GetKeyCode(), e.GetRepeatCount());
		return e.getKeyCode();
	}

	int Application::onKeyUp(KeyUpEvent & e)
	{
		//LogInfo("Key {0} released", (char)e.GetKeyCode());
		return e.getKeyCode();
	}

	int Application::onMouseButtonDown(MouseButtonDownEvent & e)
	{
		//LogInfo("Mouse button {0} pressed", e.GetButtonCode());
		return e.getButtonCode();
	}

	int Application::onMouseButtonUp(MouseButtonUpEvent & e)
	{
		//LogInfo("Mouse button {0} released", e.GetButtonCode());
		return e.getButtonCode();
	}

	bool Application::onMouseMove(MouseMovedEvent & e)
	{
		//LogInfo("Mouse moved to: {0}, {1}", e.GetXOffset(), e.GetYOffset());
		return true;
	}

	bool Application::onMouseScroll(MouseScrollEvent & e)
	{
		//LogInfo("Mouse scrolled to {0}, {1}", e.GetXOffset(), e.GetYOffset());
		return true;
	}

	Window* Application::getWindow()
	{
		return m_appWindow.get();
	}

	Application::~Application()
	{
		m_appWindow->close();

		m_windowsSystem->stop(Engine::SystemSignal::Windows);
		m_imGui->stop(Engine::SystemSignal::Windows);
		m_layerStack->stop(Engine::SystemSignal::None);	
		m_timer->stop(Engine::SystemSignal::None);
		m_logger->stop(Engine::SystemSignal::None);		
		m_physWorld->stop(Engine::SystemSignal::None);
		m_logger->stop(Engine::SystemSignal::None);

		for (auto it = m_layerStack->begin(); it != m_layerStack->end(); ++it) // "kill" all gameobjects
		{
			auto layer = dynamic_cast<JsonLayer*>((*it).get());

			if (layer != nullptr)
			{
				auto gameObjects = layer->getGameObjects();

				for (auto goIt = gameObjects.begin(); goIt != gameObjects.end(); goIt++)
				{
					auto comps = goIt->second->getComponents();

					for (auto compIt = comps.begin(); compIt != comps.end(); compIt++)
					{
						(*compIt)->onDetach();
					}
				}
			}			
		}

		lua_close(lua);
	}
}
