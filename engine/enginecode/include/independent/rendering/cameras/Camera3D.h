#pragma once

/*! \file Camera3D.h
\brief Implementation of a 3D FPS camera
*/

#include "Camera.h"

namespace Engine
{
	/*! \class Camera3D
	\brief Definition of a 3D FPS camera.

		Inherits from Camera.
	*/
	class Camera3D : public Camera
	{
	private:
		glm::vec3 m_position; //!< Camera's position

		glm::vec3 m_camForward; //!< Camera's forward vector
		glm::vec3 m_camUp; //!< Camera's up vector
		glm::vec3 m_camRight; //!< Camera's right vector
		glm::vec3 m_worldUp; //!< World's up vector

		float m_yaw; //!< Rotation around X axis
		float m_pitch; //!< Rotation around Y axis

	public:
		//! Default constructor
		Camera3D() {};
		/*! Custom constructor
		\param fov field of view
		\param aspectRatio resolution
		\param nearClip closest point to a camera drawing will occur
		\param farclip furtherst point from a camera drawing will occur
		*/
		Camera3D(float fov, float aspectRatio, float nearClip, float farClip);

		void updateViewMatrix() override;

		void setPosition(const glm::vec3 pos) override;
		// Sets view and calculates new view projection \param view new view
		void setView(const glm::mat4& view) { m_view = view; m_ViewProjection = m_projection * m_view; }

		//! Sets forward vector \param forward new forward vector
		void setForward(glm::vec3 forward) { m_camForward = forward; }
		//! Sets camera up vector \param up new up vector
		void setUp(glm::vec3 up) { m_camUp = up; }
		//! Sets world up vector \param worldUp new world up vector
		void setWorldUp(glm::vec3 worldUp) { m_worldUp = worldUp; }
		//! Sets camera right vector \param right new right vector
		void setRight(glm::vec3 right) { m_camRight = right; }

		//! Sets rotation around X axis \param y new yaw
		void setYaw(float y) { m_yaw = y; }
		//! Sets rotation around Y axis \param p new pitch
		void setPitch(float p) { m_pitch = p; }

		//! Gets current rotation around X axis \return current yaw
		float getYaw() { return m_yaw; }
		//! Gets current rotation around Y axis \return current pitch
		float getPitch() { return m_pitch; }

		glm::vec3& getPosition() override;
		//! Gets current camera forward vector \return cam forward vector
		glm::vec3 getForward() { return m_camForward; }
		//! Gets current camera up vector \return cam up vector
		glm::vec3 getUp() { return m_camUp; }
		//! Gets current world up vector \return world up vector
		glm::vec3 getWorldUp() { return m_worldUp; }
		//! Gets current camera right vector \return cam right vector
		glm::vec3 getRight() { return m_camRight; }
	};
}
