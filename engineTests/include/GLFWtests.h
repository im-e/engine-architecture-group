#pragma once

#define NG_PLATFORM_WINDOWS

#include <gtest/gtest.h>

#include <memory>

#include "core/application.h"
#include "platform/GLFW_InputPoller.h"
#include "platform/GLFW_KeyCodes.h"
#include "platform/OpenGL/OpenGL_Graphics.h"
#include "platform/OpenGL/GLFWWindowsSys.h"
#include "windows/WinPlatformWindow.h"

Engine::InputPoller* poller;
std::shared_ptr<Engine::WindowsSys> windowsSys;