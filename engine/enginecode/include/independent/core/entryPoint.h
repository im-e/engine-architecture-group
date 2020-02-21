/*! \file entryPoint.h
\brief Beginning point of the engine
*/

/*******************
	Application	
	start point
*******************/

#pragma once

#include "core/application.h"
#include "systems/debugging/Profiler.h"

extern Engine::Application* Engine::startApplication();


extern "C"
{
#include "../lua/lua.h" // Why this way??? Is my premake correct?
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"
}

int main(int argc, char** argv)
{
	auto application = Engine::startApplication();

#ifdef NG_DEBUG
	NG_PROFILER_BEGIN_SESSION("Run", "../testing/Run_Profiler.json");
#endif

	application->run();

#ifdef NG_DEBUG
	NG_PROFILER_END_SESSION();
#endif

	delete application;

	return 0;
}

/*! \mainpage DMU engine
Game engine using C++ and OpenGL. Made by Jan Kalinowski (p17179637).

Engine provides debugging functionality, such as logging and timing events. The code's been abstracted and would easily be translated to work with various platforms and graphics libraries.

Current controls are as follows:

---WASD - move 3D camera around

---IJKL - move middle cube around

---ZXC - rotate middle cube (note: vectors are not updated; cube will always move towards its local-space vectors)

---Arrows - move 2D camera

---Click & hold left mouse button to rotate 3D camera. Mouse cursor has been locked in a single position - press ESC to regain control of your mouse.

Currently available macros:

---LogInfo - logs messages (green text)

---LogWarn - logs warnings (yellow text)

---LogError - logs errors (red text)

---Time_Now - retrieves current time; useful for timing event)

---Time_Point - marks a new timepoint

---ResourceManagerInstance - get instance of resource manager (access all resources of the game engine)

---Bit(i) - shifts bit i number of times to the left

How to profile the engine:

Switch the engine to DEBUG mode. Specify one of the following in a scope you want to profile.

---NG_PROFILER_BEGIN_SESSION - marks beginning of profiling session

---NG_PROFILER_END_SESSION - marks end of profiling session

---NG-PROFILER_SCOPE - indicates that a scope will be profiled

---NG_PROFILER_FUNCTION - indicates that a function (in fact: still a scope) will be profiled

Keycodes are adapted from GLFW. They are the same, apart from changing their prefix to NG.

In order to run unit tests, switch startup project to EngineTests & switch configuration to TEST. For other testing data,
please go to testing/ folder.

Developed by Jan Kalinowski (p17179637)
*/
