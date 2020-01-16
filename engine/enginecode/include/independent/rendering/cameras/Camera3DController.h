#pragma once

/*! \file Camera3DController.h
\brief Provides implementation of controller for 3D cameras
*/

#include "CameraController.h"
#include "Camera3D.h"


namespace Engine
{
	/*! \class Camera3DController
	\brief Definition of a controller for camera 3D

		Inherits from CameraController
	*/
	class Camera3DController : public CameraController
	{
	private:
		glm::vec2 m_lastMousePosition; //!< Last mouse position in a window
		glm::vec3 m_position; //!< Position of a controller

		std::shared_ptr<Camera3D> m_camera; //!< Pointer to a camera

		float m_rotationSpeed; //!< Rotation speed
	public:
		//! Default constructor
		Camera3DController() {};
		//! Custom constructor \param translationSpeed movement speed \param rotationSpeed rotation speed
		Camera3DController(float translationSpeed, float rotationSpeed);

		void onUpdate(float timestep) override;
		void init(float fov, float aspectRatio, float nearClip, float farClip) override;
		void onEvent(Event& e) override;

		// Inherited via CameraController
		std::shared_ptr<Camera> getCamera() override { return m_camera; }
	};
}