#include "engine_pch.h"
#include "rendering/cameras/Camera2DController.h"

namespace Engine
{
	Camera2DController::Camera2DController(float translationSpeed)
	{
		m_translationSpeed = translationSpeed;
	}

	void Camera2DController::onUpdate(float timestep)
	{
		if (InputPoller::isKeyPressed(NG_KEY_UP))
		{
			m_position.x += -sin(glm::radians(m_rotation)) * m_translationSpeed * timestep;
			m_position.y += cos(glm::radians(m_rotation)) * m_translationSpeed * timestep;
		}
		if (InputPoller::isKeyPressed(NG_KEY_LEFT))
		{
			m_position.x -= cos(glm::radians(m_rotation)) * m_translationSpeed * timestep;
			m_position.y -= sin(glm::radians(m_rotation)) * m_translationSpeed * timestep;
		}
		if (InputPoller::isKeyPressed(NG_KEY_DOWN))
		{
			m_position.x += sin(glm::radians(m_rotation)) * m_translationSpeed * timestep;
			m_position.y -= cos(glm::radians(m_rotation)) * m_translationSpeed * timestep;
		}
		if (InputPoller::isKeyPressed(NG_KEY_RIGHT))
		{
			m_position.x += cos(glm::radians(m_rotation)) * m_translationSpeed * timestep;
			m_position.y += sin(glm::radians(m_rotation)) * m_translationSpeed * timestep;

		}

		m_camera->setPosition(m_position);
	}

	void Camera2DController::init(float left, float right, float bottom, float top)
	{
		m_camera = std::make_shared<Camera2D>(left, right, bottom, top);

		m_position = m_camera->getPosition();
		m_rotation = m_camera->getRotation();
	}

	void Camera2DController::onEvent(Event & e)
	{
	}
}