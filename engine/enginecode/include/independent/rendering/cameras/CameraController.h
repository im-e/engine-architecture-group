#pragma once

/*! \file CameraController.h
\brief Implementation of base class for camera controllers
*/

#include <memory>
#include "events/Event.h"
#include "events/MouseButtonEvent.h"
#include "platform/InputPoller.h"
#include "Camera.h"
#include "Camera2D.h"
#include "Camera3D.h"

#ifdef NG_PLATFORM_WINDOWS
	#include "platform/GLFW_KeyCodes.h"
#endif

namespace Engine
{
	/*! \class CameraController
	\brief Base definition of camera controller
	*/
	class CameraController
	{
	protected:
		float m_translationSpeed; //!< Translation speed of a camera

	public:
		//! Gets camera associated with a controller \return camera
		virtual std::shared_ptr<Camera> getCamera() = 0;
		//! Updates camera \param timestep delta time
		virtual void onUpdate(float timestep) = 0;
		//! Initializes camera 
		virtual void init(float, float, float, float) = 0;
		//! Called when an event occurs \param e event occurred
		virtual void onEvent(Event& e) = 0;

		//! Creates 2D controller \param translationSpeed moving speed of a camera
		static CameraController* create2DController(float translationSpeed);
		//! Creates 3D controller \param translationSpeed moving speed of a camera \param rotationSpeed rotation speed of a camera
		static CameraController* create3DController(float translationSpeed, float rotationSpeed);
	};
}