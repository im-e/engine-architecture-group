#include "engine_pch.h"
#include "rendering/cameras/Camera3D.h"

namespace Engine
{
	void Camera3D::updateViewMatrix()
	{
		glm::vec3 forward;
		forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		forward.y = sin(glm::radians(m_pitch));
		forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_camForward = glm::normalize(forward);

		m_camRight = glm::normalize(glm::cross(m_camForward, m_worldUp));
		m_camUp = glm::normalize(glm::cross(m_camRight, m_camForward));

		setView(glm::lookAt(m_position, m_position + m_camForward, m_camUp));
	}

	Camera3D::Camera3D(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
		m_position = glm::vec3(0.0f, 0.0f, -4.5f);

		m_camForward = glm::vec3(0.0f, 0.0f, -1.0f);
		m_camUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_worldUp = m_camUp;

		m_yaw = 90.f;
		m_pitch = 0.0f;

		updateViewMatrix();
	}

	void Camera3D::setPosition(const glm::vec3 pos)
	{
		m_position = pos;
		updateViewMatrix();
	}

	glm::vec3& Camera3D::getPosition()
	{
		return m_position;
	}
}