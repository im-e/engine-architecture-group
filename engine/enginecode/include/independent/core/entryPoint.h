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

*/
