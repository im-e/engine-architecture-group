/*! \file application.h
\brief Backbone of the engine
*/
#pragma once

#ifdef NG_DEBUG //display memory leaks if in debug mode
	#define _CRTDBG_MAP_ALLOC  
	#include <stdlib.h>  
	#include <crtdbg.h> 
#endif

#include <memory>
#include <vector>

#include "systems/Timer.h"
#include "systems/Log.h"
#include "systems/ImGuiSystem.h"

#include "events/Event.h"
#include "events/WindowEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseButtonEvent.h"
#include "events/MouseEvent.h"

#include "rendering/layers/LayerStack.h"
#include "platform/WindowsSys.h"
#include "windows/window.h"

namespace Engine {

	/*!
	\class Application
	\brief Fundamental class of the engine. A singleton which runs the game loop infinitely.
	
		Provides implementation and logic handling of the entire program. Is capable of logging messages, measuring time and providing
		API agnostic feature implementation (e.g. graphics), so that it can work with multiple platforms or graphics libraries.
	*/

	class Application
	{
	protected:
		Application(); //!< Constructor

		//! Pointer to stack of layers
		std::shared_ptr<LayerStack> m_layerStack;

	private:
		static Application* s_instance; //!< Singleton instance of the application
		//! Pointer to Log object
		std::shared_ptr<Log> m_logger;
		//! Pointer to Timer object
		std::shared_ptr<Timer> m_timer;
		//! Pointer to ImGuiSystem
		std::shared_ptr<ImGuiSystem> m_imGui;

#ifdef NG_PLATFORM_WINDOWS
		std::shared_ptr<WindowsSys> m_windowsSystem; //!< Pointer to Windows platform
#endif
		std::unique_ptr<Window> m_appWindow; //!< Pointer to a Window
		//! Is the application running?
		bool m_running = true;
		//! FPS count
		float m_fps;
		//! Timestep
		float m_timestep;

	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		void onEvent(Event& e); //!< Function handling events

		bool onClose(WindowCloseEvent& e); //!< Gets called on window close event
		bool onResize(WindowResizeEvent& e); //!< Gets called on window resize event
		bool onLostFocus(WindowLostFocusEvent& e); //!< Gets called when user clicks somewhere else than on the window
		bool onWindowMoved(WindowMovedEvent& e); //!< Gets called when user moves a window

		int onKeyDown(KeyDownEvent& e); //!< Called when users press a key
		int onKeyHold(KeyHoldEvent& e); //!< Called when users hold a key
		int onKeyUp(KeyUpEvent& e); //!< Called when users release a key

		int onMouseButtonDown(MouseButtonDownEvent& e); //!< Called when users press a mouse button
		int onMouseButtonUp(MouseButtonUpEvent& e); //!< Called when users release a mouse button

		bool onMouseMove(MouseMovedEvent& e); //!< Called when users move mouse
		bool onMouseScroll(MouseScrollEvent& e); //!< Called when users scroll mouse

		Window* getWindow(); //!< Returns current window \return currently opened window

		void run(); //!< Main loop
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook

}