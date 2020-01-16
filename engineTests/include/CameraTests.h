#pragma once

#include <gtest/gtest.h>
#include <memory>

#include "rendering/cameras/Camera.h"
#include "rendering/cameras/Camera2D.h"
#include "rendering/cameras/Camera3D.h"
#include "rendering/cameras/CameraController.h"
#include "rendering/cameras/Camera2DController.h"
#include "rendering/cameras/Camera3DController.h"

std::shared_ptr<Engine::CameraController> m_camera2D;
std::shared_ptr<Engine::CameraController> m_camera3D;

float m_translationSpeed = 1.0f;
float m_rotationSpeed = 1.0f;