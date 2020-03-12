#pragma once

/*! \file Camera2D.h
\brief Implementation of a 2D orthographic camera
*/

#include "Camera.h"

namespace Engine
{
	/*! \class Camera2D
	\brief Definition of a 2D orthographic camera.
	
		Inherits from Camera.
	*/
	class Camera2D : public Camera
	{
	protected:
		glm::vec3 m_position; //!< Camera position
		float m_rotation; //!< Camera rotation

	public:
		//! Default constructor
		Camera2D() {};
		/*! Custom constructor
		\param left left boundary of a view
		\param right right boundary of a view
		\param bottom bottom boundary of a view
		\param top top boundary of a view
		*/
		Camera2D(float left, float right, float bottom, float top);
		void updateViewMatrix() override;

		void setPosition(const glm::vec3 pos) override;
		//! Sets rotation of a camera \param r new rotation
		void setRotation(float r){ m_rotation = r; }

		//! Gets current camera rotation \return current rotation
		float getRotation() { return m_rotation; }
		glm::vec3& getPosition() override { return m_position; }
	};
}