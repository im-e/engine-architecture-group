#pragma once

/*! \file Camera2DController.h
\brief Provides implementation of controller for 2D cameras
*/

#include "CameraController.h"
#include "Camera2D.h"

namespace Engine
{
	/*! \class Camera2DController
	\brief Definition of a controller for camera 2D
	
		Inherits from CameraController
	*/
	class Camera2DController : public CameraController
	{
	private:
		glm::vec3 m_position; //!< Position
		float m_rotation; //!< Rotation

		std::shared_ptr<Camera2D> m_camera; //!< Pointer to a camera
	public:
		//! Default constructor
		Camera2DController() {};
		//! Custom constructor \param translationSpeed movement speed of a camera
		Camera2DController(float translationSpeed);
		void onUpdate(float timestep) override;
		void init(float left, float right, float bottom, float top) override;
		void onEvent(Event& e) override;

		// Inherited via CameraController
		std::shared_ptr<Camera> getCamera() override { return m_camera; }
	};
}