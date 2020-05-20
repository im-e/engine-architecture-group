# CW2019

Log: https://docs.google.com/spreadsheets/d/1QTLbJglgGTWWotg6pedQUjVd7UmJ3MKod_Wl70uLaog/edit#gid=1094589293

Meetings: https://docs.google.com/document/d/1nvCUfFUZRD1n--IuPNlEN4mOXbdCRSbSYgd-AgD8Ctc/edit

Game engine using C++ and OpenGL.

Authors:

Jan Kalinowski

Charlie Jenkinson

Reece Carey

Thomas Stillman-Allbones

Alfie Barton

Lily Burgham

Engine provides debugging functionality, such as logging and timing events. The code's been abstracted and would easily be translated to work with various platforms and graphics libraries.

It is capable of:

-- Loading complex models with ASSIMP

-- Loading resources asynchronously using multithreading

-- Controlling AI with Lua scripts with dynamic pathing

-- Creating levels with editor created in ImGui utilizing GameObject and ECS

-- Saving created levels to json file

-- Loading created levels from json files

-- Editing 2D and 3D layers with level editor

-- Tessellating geometries

-- Applying postprocessing effects

-- Applying gravity to rigidbodies

-- Batch rendering text 
 
-- Playing 2D and 3D sounds with FMOD

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
      
Doxygen documentation is fully generated - you can find it under doc/index.html

In order to run unit tests, switch startup project to EngineTests & switch configuration to DEBUG.
