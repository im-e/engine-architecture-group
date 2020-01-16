#include "engine_pch.h"
#include "rendering/cameras/Camera2D.h"

namespace Engine
{
	Camera2D::Camera2D(float left, float right, float bottom, float top)
	{
		m_projection = glm::ortho(left, right, bottom, top);
		m_view = glm::mat4(1.0f);
		m_ViewProjection = m_view * m_projection;

		m_position = glm::vec3(0.0f);
		m_rotation = 0.0f;
	}

	void Camera2D::updateViewMatrix()
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_position);
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		m_view = translate * rotate;

		m_ViewProjection = m_view * m_projection;
	}

	void Camera2D::setPosition(const glm::vec3 pos)
	{
		m_position = pos;
		updateViewMatrix();
	}
}