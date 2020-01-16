#pragma once

#include <gtest/gtest.h>

#include <variant>

#include "platform/OpenGL/OpenGL_Graphics.h"
#include "windows/WinPlatformWindow.h"

#include "resources/ResourceManager.h"
#include "rendering/ShaderData.h"
#include "rendering/Material.h"
#include "rendering/renderer/Renderer.h"
#include "rendering/renderer/OpenGLRenderCommands.h"

Engine::UniformElement uboElement(Engine::ShaderDataType::Float3, false);

Engine::UniformLayout layout = 
{
	Engine::ShaderDataType::Float3,
	Engine::ShaderDataType::Float3
};

std::shared_ptr<Engine::Renderer> m_renderer3D;
std::shared_ptr<Engine::Renderer> m_renderer2D;