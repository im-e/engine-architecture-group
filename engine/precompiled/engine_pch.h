/*! \file engine_pch.h
*\brief A list of header files to be precompiled in order to reduce compile times
*/

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>

#include "systems/debugging/Profiler.h"
#include "systems/Log.h"

extern "C"
{
#include "lua.h" 
#include "lualib.h"
#include "lauxlib.h"
}

#include <../LuaBridge/Source/LuaBridge/LuaBridge.h>

