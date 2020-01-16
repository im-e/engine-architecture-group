#pragma once

#include "gtest/gtest.h"

#include "platform/OpenGL/OpenGL_Graphics.h"
#include "windows/WinPlatformWindow.h"

#include "resources/ResourceManager.h"
#include "resources/OpenGLTextures.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

std::shared_ptr<Engine::Texture> testTexture;
std::shared_ptr<Engine::Shader> testShader;