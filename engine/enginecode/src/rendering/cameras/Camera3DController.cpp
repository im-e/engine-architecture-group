#include "engine_pch.h"
#include "rendering/cameras/Camera3DController.h"

namespace Engine
{
	Camera3DController::Camera3DController(float translationSpeed, float rotationSpeed)
	{
		m_translationSpeed = translationSpeed;
		m_rotationSpeed = rotationSpeed;
		m_lastMousePosition = glm::vec2(0.0f, 0.0f);
	}

	void Camera3DController::onUpdate(float timestep)
	{
		float velocity = m_translationSpeed * timestep;

		if (InputPoller::isKeyPressed(NG_KEY_W))
		{
			m_position += m_camera->getForward() * velocity;
		}
		if (InputPoller::isKeyPressed(NG_KEY_A))
		{
			m_position -= m_camera->getRight() * velocity;
		}
		if (InputPoller::isKeyPressed(NG_KEY_S))
		{
			m_position -= m_camera->getForward() * velocity;
		}
		if (InputPoller::isKeyPressed(NG_KEY_D))
		{
			m_position += m_camera->getRight() * velocity;
		}
		if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_LEFT))
		{
			glm::vec2 currentMousePos = InputPoller::getMousePosition();
			glm::vec2 mouseDelta = currentMousePos - m_lastMousePosition;

			m_camera->setYaw(m_camera->getYaw() + mouseDelta.x * m_rotationSpeed * timestep);
			m_camera->setPitch(m_camera->getPitch() - mouseDelta.y * m_rotationSpeed * timestep);

			if (m_camera->getPitch() > 89.0f)
				m_camera->setPitch(89.0f);
			if (m_camera->getPitch() < -89.0f)
				m_camera->setPitch(-89.0f);

			m_lastMousePosition = currentMousePos;
		}

		m_camera->setPosition(m_position);
	}

	void Camera3DController::init(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_camera = std::make_shared<Camera3D>(fov, aspectRatio, nearClip, farClip);
		m_position = m_camera->getPosition();
	}

	void Camera3DController::onEvent(Event & e)
	{
	}

}